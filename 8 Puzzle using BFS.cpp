// 8 Puzzle using BFS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <iostream>
#include <algorithm>
#include <queue>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <memory>


class Node {
public:
	std::vector<Node*> children;
	std::vector<int> puzzle;
	Node* parent;

public:
	Node() {};

	Node(std::vector<int> _puzzle, Node* _parent)
		: puzzle(_puzzle), parent(_parent)
	{}

	void printPuzzle(); //Print vector in puzzle format
	int blankTile(); //To find zero's location
	// Functions to move the tiles
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	void bfs(std::vector<int> const&, std::vector<int> const&);
	friend bool contains(std::queue<Node*> q, Node* n);

};

void Node::printPuzzle() {
	int count = 0;
	for (auto i : puzzle) {
		if (count % 3 == 0)
			std::cout << std::endl;
		std::cout << i << ' ';
		count++;
	}
}

int Node::blankTile() {

	//std::vector<int>::iterator it;
	auto it = std::find(puzzle.begin(), puzzle.end(), 0);
	auto z = std::distance(puzzle.begin(), it);
	return (int)z;
}

void Node::moveUp() {
	int zPos = blankTile();
	std::vector<int> temp = puzzle;
	if (zPos != 0 && zPos != 1 && zPos != 2)
		std::swap(temp[zPos], temp[zPos - 3]);
	Node* child = new Node(temp, this);
	children.push_back(child);
}

void Node::moveDown() {
	int zPos = blankTile();
	std::vector<int> temp = puzzle;
	if (zPos != 6 && zPos != 7 && zPos != 8)
		std::swap(temp[zPos], temp[zPos + 3]);
	Node* child = new Node(temp, this);
	children.push_back(child);
}

void Node::moveRight() {
	int zPos = blankTile();
	std::vector<int> temp = puzzle;
	if (zPos != 2 && zPos != 5 && zPos != 8)
		std::swap(temp[zPos], temp[zPos + 1]);
	Node* child = new Node(temp, this);
	children.push_back(child);
}

void Node::moveLeft() {
	int zPos = blankTile();
	std::vector<int> temp = puzzle;
	if (zPos != 0 && zPos != 3 && zPos != 6)
		std::swap(temp[zPos], temp[zPos - 1]);
	//std::shared_ptr<Node> child = std::make_shared<Node>(temp, this);
	Node* child = new Node(temp, this);
	children.push_back(child);
}

void Node::bfs(std::vector<int> const& begin, std::vector<int> const& end) {
	std::vector<int> start = begin;
	std::vector<int> goal = end;
	bool goalReached = false;
	std::queue<Node*> openList;
	std::queue<Node*> closedList;
	std::vector<Node*> solution;

	Node initnode = Node(start, NULL);
	openList.push(&initnode);
	while (!openList.empty() && !goalReached) {
		Node* current = openList.front();
		closedList.push(current);
		openList.pop();
		current->moveUp();
		current->moveDown();
		current->moveRight();
		current->moveLeft();

		for (auto i : current->children) {
			Node* child = i;
			int a = 0;

			if (child->puzzle == goal) {
				std::cout << "Solution Found." << std::endl;

				solution.push_back(child);
				while (child->parent != NULL) {
					child = child->parent;
					solution.push_back(child);
					goalReached = true;
				}
				std::reverse(solution.begin(), solution.end());
				std::cout << "Printing the solution \n" << std::endl;
				for (auto i : solution) {
					std::cout << "Node - " << a++ << std::endl;
					i->printPuzzle();
					std::cout << std::endl;
				}
			}

			if (!contains(openList, child) && !contains(closedList, child))
			{
				openList.push(child);
			}
		}
	}
	exit(0);
}

bool contains(std::queue<Node*> q, Node* n) {
	bool exist = false;
	while (!q.empty()) {
		if (q.front()->puzzle == n->puzzle)
			exist = true;
		q.pop();
	}
	return exist;
}

std::vector<int> initial_node() {
	std::vector<int> initial;
	int element;
	int choice;

	std::cout << "Enter '0' for user-defined start point or '1' for default start point : " << std::endl;
	std::cin >> choice;

	if (choice == 0) {

		std::cout << "\n Now, Enter the elements(only 0 - 8) without repetition in any order" << std::endl;
		for (int i = 0; i <= 8; i++) {
			std::cout << "Element - " << i << " : ";
			std::cin >> element;
			initial.push_back(element);
		}
		return initial;
	}
	else {

		std::vector<int> initial = { 1, 8, 2, 0, 4, 3, 7, 6, 5 };
		return initial;
	}
}

bool solvability(std::vector<int> const& puzzle) {
	// Function to find inversions in the puzzle which provides existence of a solution

	int inv = 0, x = 0, len = puzzle.size();

	for (int m = 0; m < len; m++) {
		x = puzzle.at(m);
		for (int n = m + 1; n < 9; n++) {
			if (puzzle.at(n) != 0 && x > puzzle.at(n))
				inv++;
		}
	}

	//std::cout << inv << std::endl;

	if (inv % 2 == 0)
		return 1;
	else
		return 0;
}

void print(std::vector<int> const& input)
{
	// Function to print vector variables to verify the given states
	for (int i = 0, len = input.size(); i < len; i++)
		std::cout << input.at(i) << ' ';
}

int main()
{
	std::vector<int> goal = { 1, 2, 3, 4, 5, 6, 7, 8, 0 };
	std::vector<int> start = initial_node();
	if (solvability(start)) {
		std::cout << "\nGiven starting node is solvable and the node is :" << std::endl;
		print(start);
		std::cout << "Searching for the solution... \n";
		Node n;
		n.bfs(start, goal);
	}
	else {
		std::cout << "\nSolution for the given starting node doesn't exist as it has odd inversions" << std::endl;
		exit(0);
	}

	std::cin.get();
}
