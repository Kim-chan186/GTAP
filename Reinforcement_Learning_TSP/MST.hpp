#pragma once

#ifndef MST_H
#define MST_H

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <algorithm>

#include "utilcv/Debug.h"
#include "utilcv/help_mat.h"

#include "show_TSP.h"
#include "data_structure_TSP.h"


using namespace cv;
using namespace std;


namespace TSP {

	//relaxed
	void switching(Node& n1, Node& n2, Node* n2n) {

		Node* bufp = n2.previous;
		Node* bufn = n2n;


		n1.previous->next = &n2;
		n2.previous = n1.previous;

		n1.previous = n2n->previous;
		n2n->previous->next = &n1;
		

		bufp->next = bufn;
		bufn->previous = bufp;
	}



	//2-opt
	void switching(Node& n1, Node& n2) {

		//print_link_ones(&n1);
		//print_link_ones(&n2);

		Node* n1n = n1.next;
		Node* n2n = n2.next;

		//���� ����
		n1.next->previous = nullptr;
		n2.next->previous = nullptr;
		n1.next = nullptr; 
		n2.next = nullptr;

		n2.reverse();

		n1.next = &n2;
		n2.previous = &n1;

		n1n->next = n2n;
		n2n->previous = n1n;

	}



	// 2 - opt
	bool opt_2(Node& n1, Node& n2) {
		if (n1.next == &n2 | n1.previous == &n2)
			return false;

			double before =
			costs->at<Vec2d>(n1.num, n1.next->num)[0]
			//+ rotation(n1.previous->num, n1.num, n1.next->num, n1.next->next->num)

			+ costs->at<Vec2d>(n2.num, n2.next->num)[0];
			//+ rotation(n2.previous->num, n2.num, n2.next->num, n2.next->next->num);


			double after =
			costs->at<Vec2d>(n1.num, n2.num)[0]
			//+ rotation(n1.previous->num, n1.num, n2.num, n2.previous->num) // ��� ���� ����

			+ costs->at<Vec2d>(n1.next->num, n2.next->num)[0];
			//+ rotation(n1.next->next->num, n1.next->num, n2.next->num, n2.next->next->num);

		if (before > after) {
			/*printf("bf : %d, %d, %d, %d, //", n1.previous->num, n1.num, n1.next->num, n1.next->next->num);
			printf("%d, %d, %d, %d, \n", n2.previous->num, n2.num, n2.next->num, n2.next->next->num);
			printf("af : %d, %d, %d, %d, //", n1.previous->num, n1.num, n2.num, n2.next->num);
			printf("%d, %d, %d, %d, \n", n1.next->next->num, n1.next->num, n2.next->num, n2.next->next->num);
			cout << n1.num << ", " << n2.num << " switching : " << before << " > " << after << endl << endl;
			*/

			switching_show(n1, n2);
			switching(n1, n2);

			show("2-opt");

			return true;
		}

		return false;
	}



	// ���� �˰���
	bool relaxed(Node& n1, Node& n2, int N = 1, bool _r = 0) {

		Node* n2n = &n2;	//������ ���� ���
							// n2n->previous == node2�� next

		for (int i = 0; i < N; i++) {
			if (n2n->next != &n1)
				n2n = n2n->next;
			else
				return 0;
		}


		double before =
			costs->at<Vec2d>(n1.previous->num, n1.num)[0]
			+ costs->at<Vec2d>(n2.previous->num, n2.num)[0]
			+ costs->at<Vec2d>(n2n->previous->num, n2n->num)[0];
			


		double after =
			costs->at<Vec2d>(n1.previous->num, n2.num)[0]
			+ costs->at<Vec2d>(n2n->previous->num, n1.num)[0]
			+ costs->at<Vec2d>(n2.previous->num, n2n->num)[0];


		double rbefore = 0;
		double rafter = 0;

		if (_r) {
			if (n1.previous->previous->num == n2.num) {
				return 0;
			}

			if (N == 1) {

				rbefore = rotation(n1.previous->previous->num, n1.previous->num, n1.num, n1.next->num)
					+ rotation(n2.previous->previous->num, n2.previous->num, n2.num, n2.next->num)
					+ rotation(n2.num, n2.next->num, n2.next->next->num);

				rafter = rotation(n1.previous->previous->num, n1.previous->num, n2.num, n1.num)
					+ rotation(n2.previous->previous->num, n2.previous->num, n2.next->num, n2.next->next->num)
					+ rotation(n2.num, n1.num, n1.next->num);
			}
			else {

				rbefore = rotation(n1.previous->previous->num, n1.previous->num, n1.num, n1.next->num)
					+ rotation(n2.previous->previous->num, n2.previous->num, n2.num, n2.next->num)
					+ rotation(n2n->previous->previous->num, n2n->previous->num, n2n->num, n2n->next->num);

				rafter = rotation(n1.previous->previous->num, n1.previous->num, n2.num, n2.next->num)
					+ rotation(n2n->previous->previous->num, n2n->previous->num, n1.num, n1.next->num)
					+ rotation(n2.previous->previous->num, n2.previous->num, n2n->num, n2n->next->num);
			}
			nn;
		}


		//relaxed
		if (before+ rbefore > after + rafter) {
			/*printf("	<<before>>\n");
			printf("%.f <= %d, %d \n", costs->at<Vec2d>(n1.previous->num, n1.num)[0], n1.previous->num, n1.num);
			printf("%.f <= %d, %d \n", costs->at<Vec2d>(n2.previous->num, n2.num)[0], n2.previous->num, n2.num);
			printf("%.f <= %d, %d \n\n", costs->at<Vec2d>(n2n->previous->num, n2n->num)[0], n2.num, n2n->num);
			
			printf("	<<after>>\n");
			printf("%.f <= %d, %d \n", costs->at<Vec2d>(n1.previous->num, n2.num)[0], n1.previous->num, n2.num);
			printf("%.f <= %d, %d \n", costs->at<Vec2d>(n2n->previous->num, n1.num)[0], n2.num, n1.num);
			printf("%.f <= %d, %d \n\n", costs->at<Vec2d>(n2.previous->num, n2n->num)[0], n2.previous->num, n2n->num);

			*/

			//cout << "switching : " << n1.num << " - "<< n2.num << endl << endl;

			//cout << "switching : " << before << " > " << after << endl << endl;
			/*print_link_ones(&n1);
			print_link_ones(&n2);
			printf("      ");
			print_link_ones(n2n);
			printf("\n");*/

			switching_show(n1, n2, n2n);
			switching(n1, n2, n2n);

			/*printf("      ");
			print_link_ones(&n1);
			print_link_ones(&n2);
			print_link_ones(n2n);
			printf("\n");*/
			
			
			show("relaxed");
			pause;

			return true;
		}

		return false;

	}//relaxed


	
	void Boruvka() {

		vector<Link> cost_vec;

		for (int i = 0; i < num_obstacle; i++) {
			costs->at<Vec2d>(i, i)[0] = 1e18;

			for (int j = i + 1; j < num_obstacle; j++) {

				// j to i
				int x = plist[i].x - plist[j].x;

				// (h-i) - (h-j) = -i + j
				int y = - plist[i].y + plist[j].y;


				// ���� �Ÿ�, ȸ�� ���
				// �� ���� ȸ���� 2m �̵��� ����
				// ��ũ�� 10m, ���� ��ü ȸ���ð��� ������� ����
				double cost = pow(x * x + y * y, 0.5);

				//double theta = atan2f(y, x) * xy2d;

				// Euclid
				costs->at<Vec2d>(i, j)[0] = cost; // i to j
				costs->at<Vec2d>(j, i)[0] = cost; // j to i

				// Rotate
				costs->at<Vec2d>(i, j)[1] = atan2f(-y, -x) * xy2d; // i to j
				costs->at<Vec2d>(j, i)[1] = atan2f(y, x) * xy2d;   // j to i


				///cout << "i,j : " << setw(2) << i << "  " << setw(2) << j << "  " << cost << endl;
				///cout << i << ", " << j << " :         " << costs->at<Vec2d>(i, j)[1] << endl << endl;
				Link LN(i, j, cost);

				cost_vec.emplace_back(LN);

			}//end for 2
		}//end for 1



		//cost ����
		sort(cost_vec.begin(), cost_vec.end());


		vector<Link>::iterator iter = cost_vec.begin();



		double sum = 0;

		start1();


		//Spanning_Tree�� ���� 1���� �� ������ �ݺ�
		for (int num_st = num_obstacle; num_st != 1; num_st--) {
			///cout << endl << "  num_st : " << num_st << endl;

			while (1) {
				///cout << "i,j : " << setw(2) << (*iter).i << "  " << setw(2) << (*iter).j  << "  " << (*iter).cost << endl ;

				//�� Node�� 2�� �ʰ��� ��θ� ������ ����, �̹� ���� �Ǿ����� �Ǵ�
				if (st[(*iter).i] << st[(*iter).j]) {
					sum += (*iter).cost;

					drow_boruvka((*iter).i, (*iter).j);

					iter++;

					show("Boruvka");

					break;
				}// end if

				iter++;

			} // end while

		}//end for


		/*{
			Node* end = st[0].get_end();
			Node* head = st[0].get_head();

			end->next = &st[num_obstacle];
			head->previous = &st[num_obstacle];

			st[num_obstacle].next = head;
			st[num_obstacle].previous = end;
		}*/


		Node* Nend = st[0].get_end();
		Nhead = st[0].get_head();

		//get �Լ��� ȣ�� ����, print_Hamilton ���
		Nend->next = Nhead;
		Nhead->previous = Nend;

		drow_boruvka(Nhead->num, Nend->num);

		show("Boruvka");
	}




	void TSP(string str = "relasxed", bool r=0) {
		
		int flag = 1, count = 0;

		//1-relasxed
		if (str == "relasxed" | str == "1-relasxed"){
			while (flag) {
				flag = false;
				count++;

				print_Hamilton(Nhead);

				start2(count);
				for (int y = 0; y < num_obstacle; y++) {
					for (int x = 0; x < num_obstacle; x++) {
						if (y == x) {
							continue;
						}
						else {
							flag += relaxed(st[y], st[x], 1, r);
						}
					}
				}
			}

		erase_k();
		}
		//2-relasxed
		if (str == "2-relasxed") {
			while (flag) {
				flag = false;
				count++;

				//print_Hamilton(Nhead);

				start3(count);
				for (int y = 0; y < num_obstacle; y++) {
					for (int x = 0; x < num_obstacle; x++) {
						if (y == x) {
							continue;
						}
						else {
							flag += relaxed(st[y], st[x], 2, r);
						}
					}
				}
			}

			erase_k();
		}

		//3-relasxed
		if (str == "3-relasxed") {
			while (flag) {
				flag = false;
				count++;

				//print_Hamilton(Nhead);

				start4(count);
				for (int y = 0; y < num_obstacle; y++) {
					for (int x = 0; x < num_obstacle; x++) {
						if (y == x) {
							continue;
						}
						else {
							flag += relaxed(st[y], st[x], 3, r);
						}
					}
				}
			}
		}


		//2-opt
		if (str == "2-opt") {
			while (flag) {
				flag = false;
				count++;

				//print_Hamilton(Nhead);

				start5(count);
				for (int y = 0; y < num_obstacle; y++) {
					for (int x = 0; x < num_obstacle; x++) {
						if (x == y) {
							continue;
						}
						flag += opt_2(st[y], st[x]);
					}
				}
			}
			erase_k();
		}

		return;

	}//end fBoruvka()



	//Node i*j, ��ֹ� ����, ��ֹ� ��ġ �迭
	void init(int _num_obstacle = -1, Point* _plist = nullptr) {
		
		//������ ��ֹ��� ���� ���
		if (_num_obstacle == -1) {
			cout << "int put obstacle is -1 \n";
			//���� ����
			return;
		}
		
		//��ֹ���ġ�� list�� ����Ǿ����� ���� ���
		if (_plist == nullptr) {
			cout << "\n ** MST.h error : plist is NULL!!\n\n";
			return;
		}

		num_obstacle = _num_obstacle;

		plist = _plist;

		//����, ȸ����
		costs = new Mat(Size(num_obstacle + 1, num_obstacle + 1), CV_64FC2, Scalar::all(0));

		//init, ������ ���� (���� == ó��)���� ���� ���
		st = new Node[num_obstacle + 1];
		for (int k = 0; k < num_obstacle + 1; k++) {
			st[k].num = k;
		}

	}//End init


}// End TSP 


#endif /// !EMOTION_H