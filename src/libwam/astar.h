#ifndef ASTAR_HEADER
#define ASTAR_HEADER

#include <vector>
#include <list>
#include <functional>
#include <algorithm>

#include <iostream>

class Astar {
	struct Node {
		int id, parent;
		float from_score, goal_score, final_score;
	};

	typedef std::vector< std::vector<int> > Vect2d;
	typedef std::function<float(int,int)> DistFunc;
	std::vector<Node> nodes_score;
	std::vector<float> contraintes;
    int toBlackList;
	Vect2d grid_neighbor;

	private:
		int remonteWay(int from, int to, bool withConstrains);


	public:
		Astar(std::size_t n);
        Astar() {}

        void init(std::size_t n) {
            nodes_score.resize(n);
            contraintes.clear();
            contraintes.resize(n, 0.0);
            grid_neighbor.resize(n);
        }

		void resetConstrains();
		void newPath();

        void setBL(int pos) {
            toBlackList = pos;
        }

		template< class Func>
		void inidGrid( Func neighborFunc) {
			for(std::size_t i=0; i<nodes_score.size(); ++i) {
				grid_neighbor[i] = neighborFunc(i);
				//std::cout << "node " << i << " : neighbor:" << grid_neighbor[i].size() << std::endl;
				nodes_score[i].id = i;
				nodes_score[i].parent = -1;
			}
		}

        void dijkstra(int start, bool withConstrains = false);

		template< class EuristiKfunc>
		int findWay(int start, int goal, EuristiKfunc distFunc, bool withConstrains = false) {
			std::vector<int> closed(nodes_score.size()), opened(nodes_score.size());
			std::list<int> open_liste;

			nodes_score[start].from_score = 0.0;
			nodes_score[start].final_score = nodes_score[start].from_score + distFunc(start, goal) ;


			open_liste.push_back( start );
			opened[start] = 1;

			if( toBlackList>=0 )
				closed[toBlackList] = 1;
			toBlackList=-1;

			while( !open_liste.empty() ) {
				auto it = findBestNode(open_liste.begin(), open_liste.end());
				int current = *it;
				//std::cout << "current = " << current << std::endl;


				if( current == goal) {
					return remonteWay(goal, start, withConstrains);
				}

				open_liste.erase(it);
				opened[current] = 0;
				closed[current] = 1;

				//std::vector<int>::iterator itNeighbor = grid_neighbor[current].begin();
				for(auto ipos:grid_neighbor[current]) {
				//for( ; itNeighbor != grid_neighbor[current].end(); ++itNeighbor) {
					if( closed[ipos] == 1 )
						continue;

					//std::cout << "    " << *itNeighbor << std::endl;
					float new_from_score = nodes_score[current].from_score + 1.0 + contraintes[ipos];

					if( opened[ipos] == 0 || new_from_score < nodes_score[ipos].from_score ) {
						Node& n = nodes_score[ipos];
						n.parent = current;
						n.from_score = new_from_score;
						n.final_score = n.from_score + distFunc(ipos, goal);
						if( opened[ipos] == 0 ) {
							//std::cout << "     add" << std::endl;
							open_liste.push_back( ipos );
							opened[ipos] = 1;
						}
					}
				}
			}
			return -1;
		}

		template< class IteratorType>
		IteratorType findBestNode(IteratorType b, IteratorType e) {
			IteratorType res = b;
			float minScore = nodes_score[*b].final_score;
			while( b != e) {
				float s = nodes_score[*b].final_score;
				if( s < minScore) {
					res = b;
					minScore = s;
				}
				++b;
			}
			return res;
		}

		Node& getNode(int i) {
			return nodes_score[i];
		}

};


#endif
