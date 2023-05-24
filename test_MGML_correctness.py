import MGML

import unittest

class MGMLTestCase(unittest.TestCase):
	def test_MCBM_basic(self):
		print("MGML.MCBM")
		adjx = [[1, 2], [2, 3], [1]];
		ny = 4;
		for x in range(len(adjx)):
			print(x, end='');
			print(": ", end='');
			for y in adjx[x]:
				print(y, end=' ');
			print();
		print(flush=True);

		mx = MGML.MCBM(adjx, ny);
		for y in mx:
			print(y, end=' ');
		print();
		print(flush=True);

	def test_MCBM_file(self):
		f = open('testcase_MCBM.txt', 'r');

		tc = int(f.readline().rstrip('\n'));
		for _ in range(tc):
			nx = int(f.readline().rstrip('\n'));
			ny = int(f.readline().rstrip('\n'));
			adjx = [];
			for x in range(nx):
				adjx.append([int(y) for y in f.readline().split()]);
			ans = int(f.readline().rstrip('\n'));

			mx = MGML.MCBM(adjx, ny);

			self.assertEqual(len(mx), nx);
			mx = [y for y in mx if y != -1];
			self.assertEqual(len(mx), len(set(mx)));
			self.assertEqual(len(mx), ans);

		f.close();

	def test_MWBM_basic(self):
		print("MGML.MWBM")
		weight = [
			[8, 4, 6],
			[5, 2, 3],
			[9, 4, 8]
		];
		ny = 4;
		for v in weight:
			for w in v:
				print(w, end=' ');
			print();
		print(flush=True);

		mx = MGML.MWBM(weight);
		for y in mx:
			print(y, end=' ');
		print();
		print(flush=True);
		
	def test_MWBM_file(self):
		f = open('testcase_MWBM.txt', 'r');

		tc = int(f.readline().rstrip('\n'));
		for _ in range(tc):
			n = int(f.readline().rstrip('\n'));
			weight = [];
			for x in range(n):
				weight.append([int(y) for y in f.readline().split()]);
			ans = int(f.readline().rstrip('\n'));

			mx = MGML.MWBM(weight);

			self.assertEqual(len(mx), n);
			cnt = 0;
			for x in range(n):
				y = mx[x];
				self.assertGreaterEqual(y, 0);
				self.assertLessEqual(y, n - 1);
				cnt += weight[x][y];
			self.assertEqual(cnt, ans);

		f.close();

	def test_MCM_basic(self):
		print("MGML.MCM")
		adj = [
			[1],
			[2, 4],
			[3, 1],
			[4, 2],
			[1, 3]
		];
		for u in range(len(adj)):
			print(u, end='');
			print(": ", end='');
			for v in adj[u]:
				print(v, end='');
			print();
		print(flush=True);

		m = MGML.MCM(adj);
		for v in m:
			print(v, end=' ');
		print();
		print(flush=True);
		
	def test_MCM_file(self):
		f = open('testcase_MCM.txt', 'r');

		tc = int(f.readline().rstrip('\n'));
		for _ in range(tc):
			n = int(f.readline().rstrip('\n'));
			adj = [];
			for u in range(n):
				adj.append([int(v) for v in f.readline().split()]);
			ans = int(f.readline().rstrip('\n'));

			mh = MGML.MCM(adj);

			self.assertEqual(len(mh), n);
			cnt = 0;
			for u in range(n):
				if mh[u] == -1:
					continue;
				self.assertEqual(mh[mh[u]], u);
				cnt += 1;
			self.assertEqual(cnt // 2, ans);

		f.close();

if __name__ == '__main__':
	unittest.main(verbosity=2);
