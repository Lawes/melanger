#include "astar.h"


using namespace std;

Astar::Astar(size_t n)  : 
    nodes_score(n),
    contraintes(n, 0.0),
    toBlackList(-1),
    grid_neighbor(n) 
{

}

int Astar::remonteWay(int from, int to, bool withConstrains) {
    vector<int> path;
	int current = from, test;
	path.push_back(from);

    if( from == to )
        return from;

	while((test=nodes_score[current].parent)  != to) {
		current = test;
		//cout << current << " ";
		if( test == -1) break;
		path.push_back(test);

	}

    if( withConstrains ) {
        //float c = (4.0/static_cast<float>(path.size()) + .2);
        float c = static_cast<float>(path.size());
        c = max(min(c, 10.0f), 4.0f);
        c = -0.5*(c-4.0)*(c-10.0)+1.0;
        //float c = max(20.0-static_cast<float>(path.size()), 1.0);
        //vector<int>::iterator it = path.begin();
        //for(; it != path.end(); ++it)
        for(auto ipos:path)
            contraintes[ipos] += c;
    }
	//contraintes[current] = (withConstrains?10.0/count+1.0:0.0);

	return current;

}

void Astar::dijkstra(int start, bool withConstrains) {
    std::vector<int> closed(nodes_score.size()), opened(nodes_score.size());
    std::list<int> open_liste;

    nodes_score[start].from_score = 0.0;

    open_liste.push_back( start );
    opened[start] = 1;

    if( toBlackList>=0 )
        closed[toBlackList] = 1;
    toBlackList=-1;

    while( !open_liste.empty() ) {
        int current = open_liste.back();
        open_liste.pop_back();
        opened[current] = 0;
        closed[current] = 1;

        //std::vector<int>::iterator itNeighbor = grid_neighbor[current].begin();
        //for( ; itNeighbor != grid_neighbor[current].end(); ++itNeighbor) {
        for(auto ipos:grid_neighbor[current]) {
            if( closed[ipos] == 1 )
                continue;

            //std::cout << "    " << *itNeighbor << std::endl;
            float new_from_score = nodes_score[current].from_score + 1.0 + (withConstrains?contraintes[ipos]:0.0);

            if( opened[ipos] == 0 || new_from_score < nodes_score[ipos].from_score ) {
                Node& n = nodes_score[ipos];
                n.parent = current;
                n.from_score = new_from_score;

                if( opened[ipos] == 0 ) {
                    open_liste.push_back( ipos );
                    opened[ipos] = 1;
                }
            }
        }
    }
}


void Astar::resetConstrains() {
	fill(contraintes.begin(), contraintes.end(), 0.0);
}

void Astar::newPath() {
	for(size_t i=0; i<nodes_score.size(); ++i) {
		nodes_score[i].id = i;
		nodes_score[i].parent = -1;
		nodes_score[i].from_score = 0.0;
		nodes_score[i].final_score = 0.0;
	}
}