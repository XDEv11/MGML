import MGML

import unittest
import time

class MGMLTestCase(unittest.TestCase):
	def test_MCBM_file(self):
		f = open('testcase_MCBM_woans.txt', 'r');

		tt = 0;
		tc = int(f.readline().rstrip('\n'));
		for _ in range(tc):
			nx = int(f.readline().rstrip('\n'));
			ny = int(f.readline().rstrip('\n'));
			adjx = [];
			for x in range(nx):
				adjx.append([int(y) for y in f.readline().split()]);

			b = time.time();
			mx = MGML.MCBM(adjx, ny);
			e = time.time();
			print(f'{(e - b) * 1000:.3f}', end=' ms / ');
			tt += e - b;

			self.assertEqual(len(mx), nx);
			mx = [y for y in mx if y != -1];
			self.assertEqual(len(mx), len(set(mx)));
		print();
		print('total time is', tt, 'seconds');

		f.close();

	def test_MWBM_file(self):
		f = open('testcase_MWBM_woans.txt', 'r');

		tt = 0;
		tc = int(f.readline().rstrip('\n'));
		for _ in range(tc):
			n = int(f.readline().rstrip('\n'));
			weight = [];
			for x in range(n):
				weight.append([int(y) for y in f.readline().split()]);

			b = time.time();
			mx = MGML.MWBM(weight);
			e = time.time();
			print(f'{(e - b) * 1000:.3f}', end=' ms / ');
			tt += e - b;

			self.assertEqual(len(mx), n);
			for x in range(n):
				y = mx[x];
				self.assertGreaterEqual(y, 0);
				self.assertLessEqual(y, n - 1);
		print();
		print('total time is', tt, 'seconds');

		f.close();

	def test_MCM_file(self):
		f = open('testcase_MCM_woans.txt', 'r');

		tt = 0;
		tc = int(f.readline().rstrip('\n'));
		for _ in range(tc):
			n = int(f.readline().rstrip('\n'));
			adj = [];
			for u in range(n):
				adj.append([int(v) for v in f.readline().split()]);

			b = time.time();
			mh = MGML.MCM(adj);
			e = time.time();
			print(f'{(e - b) * 1000:.3f}', end=' ms / ');
			tt += e - b;

			self.assertEqual(len(mh), n);
			for u in range(n):
				if mh[u] == -1:
					continue;
				self.assertEqual(mh[mh[u]], u);
		print();
		print('total time is', tt, 'seconds');

		f.close();

if __name__ == '__main__':
	unittest.main(verbosity=2);
