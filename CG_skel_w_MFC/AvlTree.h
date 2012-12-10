/*
 * AvlTree.h
 *
 *  Created on: May 29, 2012
 *      Author: arik
 */

#ifndef AVLTREE_H_
#define AVLTREE_H_
#include <stdlib.h>
#include <stdexcept>
#include <iostream>


class AvlTreeException: public std::exception
{
};
class AvlTreeExceptionItemNotFound: public AvlTreeException
{
};
class AvlTreeExceptionItemExist: public AvlTreeException
{
};
template<typename T>
class AvlTree
{
	class AvlTreeNode
	{
	private:
		T* element;
		AvlTreeNode* left;
		AvlTreeNode* right;
		AvlTreeNode* parent;
		int BalanceF;
		int height;
		int rank;
		int rankL;
		int rankR;
		void updateHeight()
		{
			if (left && right)
				height = (left->height > right->height ? left->height
						: right->height) + 1;
			else if (!left && !right)
				height = 0;
			else
				height = 1;
		}
		void updateRanks()
		{
			rankR = (right) ? right->getRank() : 0;
			rankL = (left) ? left->getRank() : 0;
			rank = 1 + rankL + rankR;
		}
		int getRank()
		{
			return rank;
		}
		void updateBalanceFactor()
		{
			updateHeight();
			BalanceF = (left ? left->height + 1 : 0) - (right ? right->height
					+ 1 : 0);
			updateRanks();
		}

	public:
		AvlTreeNode(const T newElemnet, AvlTreeNode* nodeParent) :
			element(new T(newElemnet)), left(NULL), right(NULL), parent(
					nodeParent), BalanceF(0), height(0), rank(0), rankL(0),
					rankR(0)
		{
		}

		~AvlTreeNode()
		{
			left = NULL;
			right = NULL;
			parent = NULL;
			delete element;
		}
		int getBF()
		{
			return BalanceF;
		}
		bool isLeaf()
		{
			return !left && !right;
		}
		int getRankL()
		{
			return rankL;
		}
		int getRankR()
		{
			return rankR;
		}
		AvlTreeNode* getNextFollowing()
		{
			AvlTreeNode* tmp = getRight();
			while (tmp->getLeft())
				tmp = tmp->getLeft();
			return tmp;
		}
		AvlTreeNode* getPrevFollowing()
		{
			AvlTreeNode* tmp = getLeft();
			while (tmp->getRight())
				tmp = tmp->getRight();
			return tmp;
		}
		T *getElement() const
		{
			return element;
		}

		AvlTreeNode *getLeft() const
		{
			return left;
		}

		AvlTreeNode *getRight() const
		{
			return right;
		}

		void setElement(T *element)
		{
			this->element = element;
		}

		void setLeft(AvlTreeNode *left)
		{
			this->left = left;
		}

		void setRight(AvlTreeNode *right)
		{
			this->right = right;
		}
		AvlTreeNode* getParent()
		{
			return parent;
		}
		void setParent(AvlTreeNode* parent)
		{
			this->parent = parent;
		}
		void updateNode()
		{
			updateBalanceFactor();
		}
	};

private:
	AvlTreeNode* root;
	AvlTreeNode* iteratorCurrent;
	AvlTreeNode* iteratorPrevious;
	int size;
	bool XOR(bool x, bool y)
	{
		return (!x && y) || (x && !y);
	}
	void clearTree(AvlTreeNode* node)
	{
		if (node && node->getLeft())
			clearTree(node->getLeft());
		if (node && node->getRight())
			clearTree(node->getRight());
		delete node;
	}
	void updateTree(AvlTreeNode* fromHere)
	{
		fromHere->updateNode();
		if (fromHere->getBF() == 2)
		{
			if (fromHere->getLeft()->getBF() >= 0)
				RollLL(fromHere);
			else if (fromHere->getLeft()->getBF() == -1)
				RollLR(fromHere);
		}
		else if (fromHere->getBF() == -2)
			if (fromHere->getRight()->getBF() <= 0)
				RollRR(fromHere);
			else if (fromHere->getRight()->getBF() == 1)
				RollRL(fromHere);
		if (fromHere->getParent())
			updateTree(fromHere->getParent());
		fromHere->updateNode();
		//		fromHere->updateNode();
	}
	void RollRR(AvlTreeNode* node)
	{
		iteratorPrevious = node;
		iteratorCurrent = iteratorPrevious->getRight();
		iteratorPrevious->setRight(iteratorCurrent->getLeft());
		if (iteratorPrevious->getRight())
			iteratorPrevious->getRight()->setParent(iteratorPrevious);
		iteratorCurrent->setLeft(iteratorPrevious);
		if (!iteratorPrevious->getParent())
			root = iteratorCurrent;
		else if (iteratorPrevious->getParent()->getRight() == iteratorPrevious)
			iteratorPrevious->getParent()->setRight(iteratorCurrent);
		else
			iteratorPrevious->getParent()->setLeft(iteratorCurrent);
		iteratorCurrent->setParent(iteratorPrevious->getParent());
		iteratorPrevious->setParent(iteratorCurrent);
		iteratorPrevious->updateNode();
		iteratorCurrent->updateNode();
		node->updateNode();
	}
	void RollRL(AvlTreeNode* node)
	{
		iteratorPrevious = node;
		iteratorCurrent = node->getRight();
		AvlTreeNode* b = iteratorCurrent->getLeft();
		AvlTreeNode* parent = node->getParent();
		iteratorCurrent->setLeft(b->getRight());
		if (iteratorCurrent->getLeft())
			iteratorCurrent->getLeft()->setParent(iteratorCurrent);
		iteratorPrevious->setRight(b->getLeft());

		if (iteratorPrevious->getRight())
			iteratorPrevious->getRight()->setParent(iteratorPrevious);
		b->setRight(iteratorCurrent);
		b->setLeft(iteratorPrevious);
		if (parent)
			root = b;
		else if (parent->getRight() == iteratorPrevious)
			parent->setRight(b);
		else
			parent->setLeft(b);
		b->setParent(iteratorPrevious->getParent());
		iteratorPrevious->setParent(b);
		iteratorCurrent->setParent(b);
		iteratorCurrent->updateNode();
		iteratorPrevious->updateNode();
		b->updateNode();
		node->updateNode();
	}
	void RollLR(AvlTreeNode* node)
	{
		iteratorPrevious = node;
		iteratorCurrent = node->getLeft();
		AvlTreeNode* b = iteratorCurrent->getRight();
		AvlTreeNode* parent = iteratorPrevious->getParent();
		iteratorPrevious->setLeft(b->getRight());
		iteratorCurrent->setRight(b->getLeft());
		b->setRight(iteratorPrevious);
		iteratorPrevious->setParent(b);
		b->setLeft(iteratorCurrent);
		iteratorCurrent->setParent(b);
		if (parent)
			root = b;
		else if (parent->getRight() == iteratorPrevious)
			parent->setRight(b);
		else if (parent->getLeft() == iteratorPrevious)
			parent->setLeft(b);
		b->updateNode();
		parent->updateNode();
		iteratorCurrent->updateNode();
		iteratorPrevious->updateNode();
		iteratorCurrent = root;
		iteratorPrevious = root;
		node->updateNode();
	}
	void RollLL(AvlTreeNode* node)
	{
		iteratorPrevious = node;
		iteratorCurrent = node->getLeft();
		iteratorPrevious->setLeft(iteratorCurrent->getRight());
		iteratorCurrent->setRight(iteratorPrevious);
		iteratorCurrent->setParent(iteratorPrevious->getParent());
		iteratorPrevious->setParent(iteratorCurrent);
		AvlTreeNode * parent = iteratorCurrent->getParent();
		if (parent)
			if (parent->getLeft() == iteratorPrevious)
				parent->setLeft(iteratorCurrent);
			else if (parent->getRight() == iteratorPrevious)
				parent->setRight(iteratorCurrent);
		iteratorCurrent->updateNode();
		iteratorPrevious->updateNode();
		parent->updateNode();
		iteratorCurrent = root;
		iteratorPrevious = root;
		node->updateNode();
	}
	T* getRankN(int i)
	{
		if (iteratorCurrent->getRankL() == (i - 1))
			return iteratorCurrent->getElement();
		else if (iteratorCurrent->getRankL() > (i - 1))
		{
			iteratorCurrent = iteratorCurrent->getLeft();
			return getRankN(i);
		}
		else
		{
			i = i - iteratorCurrent->getRankL() - 1;
			iteratorCurrent = iteratorCurrent->getRight();
			return getRankN(i);
		}
	}
public:
	AvlTree() :
		root(NULL), iteratorCurrent(NULL), iteratorPrevious(NULL), size(0)
	{
	}
	~AvlTree()
	{
		clearTree(root);
	}

	void insert(T elment)
	{
		iteratorCurrent = root;
		AvlTreeNode * newNode = new AvlTreeNode(elment, NULL);
		if (!root)
		{
			root = newNode;
			return;
		}
		T* tmp = find(&elment);
		if (tmp)
			throw AvlTreeExceptionItemExist();
		AvlTreeNode* parent = iteratorPrevious;
		newNode->setParent(parent);
		if (elment < *parent->getElement())
			parent->setLeft(newNode);
		else
			parent->setRight(newNode);
		updateTree(newNode);
		size++;
		return;
	}
	void remove(T item)
	{
		AvlTreeNode* itemToRemove = find(&item);
		if (!itemToRemove)
			throw AvlTreeExceptionItemNotFound();
		if (root == itemToRemove)
		{
			T* tmp = root->getElement();
			AvlTreeNode* prev = root->getPrevFollowing();
			root->setElement(prev->getElement());
			prev->setElement(tmp);
			itemToRemove = prev;
		}
		if (itemToRemove->isLeaf())
		{
			AvlTreeNode* parent = itemToRemove->getParent();
			parent->getLeft() == itemToRemove ? parent->setLeft(NULL)
					: parent->setRight(NULL);
			delete itemToRemove;
			updateTree(parent);
		}
		else if (XOR(itemToRemove->getLeft(), itemToRemove->getRight()))
		{
			AvlTreeNode* parent = itemToRemove->getParent();
			AvlTreeNode* singleNode =
					itemToRemove->getLeft() ? itemToRemove->getLeft()
							: itemToRemove->getRight();
			parent->getLeft() == itemToRemove ? parent->setLeft(
					itemToRemove->getRight()) : parent->setRight(
					itemToRemove->getRight());
			singleNode->setParent(parent);
			delete (itemToRemove);
			updateTree(parent);
		}
		else
		{
			AvlTreeNode* following = itemToRemove->getNextFollowing();
			T* tmpElement = following->getElement();
			following->setElement(itemToRemove->getElement());
			itemToRemove->setElement(tmpElement);
			itemToRemove = following;
			AvlTreeNode* parent = itemToRemove->getParent();
			parent->setRight(itemToRemove->getRight());
			delete (itemToRemove);
			updateTree(parent);
		}
		size--;
	}
	T* find(T* item)
	{
		iteratorCurrent = root;
		while (iteratorCurrent)
		{
			if (!iteratorCurrent)
				return NULL;
			iteratorPrevious = iteratorCurrent;
			if (*iteratorCurrent->getElement() < *item)
				iteratorCurrent = iteratorCurrent->getRight();
			else if (*iteratorCurrent->getElement() > *item)
				iteratorCurrent = iteratorCurrent->getLeft();
			else if (*iteratorCurrent->getElement() == *item)
				return iteratorCurrent->getElement();
		}
		return NULL;
	}
	T* getTheN(int i)
	{
		iteratorCurrent = root;
		return getRankN(i);
	}
	int getSize()
	{
		return size;
	}
};


#endif /* AVLTREE_H_ */
