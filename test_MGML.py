import MGML

import unittest

class MGMLTestCase(unittest.TestCase):
	def test_MCBM(self):
		print("MGML.MCBM")
		adjx = ((1, 2), (2, 3), (1, ));
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

	def test_MWBM(self):
		print("MGML.MWBM")
		weight = (
			(8, 4, 6),
			(5, 2, 3),
			(9, 4, 8)
		);
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

	def test_MCM(self):
		print("MGML.MCM")
		adj = (
			(1, ),
			(2, 4),
			(3, 1),
			(4, 2),
			(1, 3)
		);
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


if __name__ == '__main__':
	unittest.main(verbosity=2);
