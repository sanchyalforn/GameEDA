int bfs_heli(int hId, Posicio posInici, int orientacioInici, Posicio posDesti) {

	int inici = po2id(posInici, orientacioInici);
	int desti = p2id(posDesti);

	int dx[4] = { -1, 0, 1, 0 };
	int dy[4] = { 0, -1, 0, 1 };
	vector<bool> visitat = vector<bool>(nMapa * 4, false);
	queue<int> q;
	visitat[desti] = true;

	q.push(desti);

	while (!q.empty()) {
		int id = q.front();
		Posicio pos = id2p(id);
		int o = id2o(id);
		q.pop();
		Posicio p = Posicio(pos.x + dx[o], pos.y + dy[o]);
		if (valid(p))
			if (visitar_posicio_bfs_helis(hId, AVANCA1, po2id(p, o), visitat, q, inici)) {
				return AVANCA1;
			}

			p = Posicio(p.x + dx[o], p.y + dy[o]);
			if (valid(p))
				if (visitar_posicio_bfs_helis(hId, AVANCA1, po2id(p, o), visitat, q, inici))
					return AVANCA2;

				if (visitar_posicio_bfs_helis(hId, AVANCA1, po2id(pos, (o + 1) % 4), visitat, q, inici))
					return RELLOTGE;

				if (visitar_posicio_bfs_helis(hId, CONTRA_RELLOTGE, po2id(pos, (o - 1 + 4) % 4), visitat, q, inici))
					return CONTRA_RELLOTGE;
			}
			return -1;
}

bool visitar_posicio_bfs_helis(int hId, int instruccio, int id, vector<bool> & visitat, queue<int> &q, int &inici) {
	if (not visitat[id] and validHeli[hId][id % nMapa]) {
		visitat[id] = true;
		q.push(id);
		if (id == inici) {
			return true;
		}
	}
	return false;
}
