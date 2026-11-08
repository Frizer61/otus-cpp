import collections
import pathlib
import re
import shutil
import subprocess
import sys
import tempfile
import unittest


BINARY = str(pathlib.Path(sys.argv.pop(1)).resolve())


class BayanTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = pathlib.Path(self.temp.name).resolve()

    def file(self, name, data):
        path = self.root / name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(data)
        return str(path)

    def run_bayan(self, *args):
        return subprocess.run(
            [BINARY, *map(str, args)], cwd=self.root,
            capture_output=True, text=True)

    def groups(self, *args):
        result = self.run_bayan(*args)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(result.stderr, "")
        output = result.stdout.strip()
        if not output:
            return set()
        groups = [part.splitlines() for part in output.split("\n\n")]
        for group in groups:
            self.assertGreaterEqual(len(group), 2)
            self.assertEqual(len(group), len(set(group)))
            self.assertTrue(all(pathlib.Path(p).is_absolute() for p in group))
        return {frozenset(group) for group in groups}

    def test_hashes_and_blocks(self):
        a = self.file("a.txt", b"Hello, World!\x00\xff")
        b = self.file("b.txt", b"Hello, World!\x00\xff")
        c = self.file("c.txt", b"Hello, World!\x00\xff")
        d = self.file("d.txt", b"Hello, C++++!\x00\xff")
        e = self.file("e.txt", b"Hello, C++++!\x00\xff")
        self.file("unique.txt", b"Hello, Python!!")
        # Совпадающий после дополнения нулями блок, но другой размер файла.
        self.file("short.txt", b"Hello, World!\x00\xff\x00")
        expected = {frozenset([a, b, c]), frozenset([d, e])}
        for algorithm in ["crc32", "md5"]:
            for block in [1, 5, 15, 64]:
                with self.subTest(hash=algorithm, block=block):
                    self.assertEqual(self.groups("--hash", algorithm,
                                                "--block-size", block), expected)

    def test_depth_exclusions_and_overlapping_roots(self):
        a = self.file("a", b"same")
        b = self.file("b", b"same")
        c = self.file("sub/c", b"same")
        d = self.file("sub/deep/d", b"same")
        e = self.file("sub-other/e", b"same")
        self.assertEqual(self.groups(), {frozenset([a, b])})
        self.assertEqual(self.groups("--depth", 1), {frozenset([a, b, c, e])})
        self.assertEqual(self.groups("--depth", -1), {frozenset([a, b, c, d, e])})
        self.assertEqual(self.groups("--depth", -1, "--exclude", "sub/"),
                         {frozenset([a, b, e])})
        self.assertEqual(self.groups("--scan", ".", "sub", "--scan", ".",
                                     "--depth", -1), {frozenset([a, b, c, d, e])})
        self.assertEqual(self.groups("--scan", "sub/deep", "--exclude", "sub"), set())
        self.assertEqual(self.groups("--depth", -1, "--exclude", "sub",
                                     "--exclude", "sub-other"), {frozenset([a, b])})

    def test_masks(self):
        a = self.file("report.TXT", b"same")
        b = self.file("copy.txt", b"same")
        c = self.file("one.BIN", b"same")
        self.file("skip.dat", b"same")
        self.assertEqual(self.groups("--mask", "*.txt"), {frozenset([a, b])})
        self.assertEqual(self.groups("--mask", "*.txt", "--mask", "???.bin"),
                         {frozenset([a, b, c])})
        self.assertEqual(self.groups("--mask", "*.missing"), set())
        d = self.file("file[1].txt", b"literal")
        e = self.file("file[2].txt", b"literal")
        self.assertEqual(self.groups("--mask", "file[?].txt"), {frozenset([d, e])})

    def test_minimum_and_empty_files(self):
        a = self.file("a", b"")
        b = self.file("b", b"")
        c = self.file("c", b"x")
        d = self.file("d", b"x")
        e = self.file("e", b"xx")
        f = self.file("f", b"xx")
        self.assertEqual(self.groups(), {frozenset([e, f])})
        self.assertEqual(self.groups("--min-size", 0),
                         {frozenset([a, b]), frozenset([c, d]), frozenset([e, f])})
        self.assertEqual(self.groups("--min-size", 3), set())

    def test_symlinks(self):
        a = self.file("a", b"same")
        b = self.file("b", b"same")
        (self.root / "link").symlink_to(a)
        (self.root / "loop").symlink_to(self.root, target_is_directory=True)
        self.assertEqual(self.groups("--depth", -1), {frozenset([a, b])})

    def test_invalid_arguments(self):
        for args in [("--block-size", "0"), ("--block-size", "-1"),
                     ("--min-size", "-1"), ("--depth", "-2"),
                     ("--hash", "sha256"), ("--scan", "missing"),
                     ("--unknown",), ("--depth", "abc")]:
            with self.subTest(args=args):
                result = self.run_bayan(*args)
                self.assertNotEqual(result.returncode, 0)
                self.assertEqual(result.stdout, "")
                self.assertTrue(result.stderr)
        self.assertEqual(self.run_bayan("--help").returncode, 0)

    @unittest.skipUnless(sys.platform == "linux" and shutil.which("strace"),
                         "Read counts require Linux and strace")
    def test_lazy_reads(self):
        a = self.file("a", b"AAAA1111tail")
        b = self.file("b", b"AAAA2222tail")
        c = self.file("c", b"AAAA1111tail")
        d = self.file("d", b"BBBB1111tail")
        unique = self.file("unique", b"different-size")
        trace = self.root / "trace.log"
        result = subprocess.run(
            ["strace", "-yy", "-e", "trace=read", "-o", str(trace), BINARY,
             "--scan", str(self.root), "--mask", "a", "b", "c", "d", "unique",
             "--block-size", "4"], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        reads = collections.defaultdict(list)
        for path, size in re.findall(r'read\(\d+<([^>]+)>,.*\)\s+= (\d+)',
                                     trace.read_text()):
            if path in [a, b, c, d, unique]:
                reads[path].append(int(size))
        self.assertEqual(reads[a], [4, 4, 4], trace.read_text())
        self.assertEqual(reads[b], [4, 4])
        self.assertEqual(reads[c], [4, 4, 4])
        self.assertEqual(reads[d], [4])
        self.assertEqual(reads[unique], [])


if __name__ == "__main__":
    unittest.main()
