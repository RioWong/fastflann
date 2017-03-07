/***********************************************************************
* Software License Agreement (BSD License)
*
* Copyright 2017  Wolfgang Brandenburger. All rights reserved.
*
* THE BSD LICENSE
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
* NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*************************************************************************/

#ifndef DATASTRUCTURES_H_
#define DATASTRUCTURES_H_

namespace flann
{

template <typename T> 
class balancedTree 
{
public:

	/**
	* 
	*/
	T value;
	bool occupied;

	int depth;

	balancedTree* parent;

	balancedTree<T>* nodel;
	balancedTree<T>* noder;

	balancedTree(balancedTree<T>* parent_) 
	{
		value = NAN;
		depth = 0;

		occupied = 0;

		parent = parent_;

		nodel = NULL;
		noder = NULL;
	}

	~balancedTree(void) 
	{
		if (nodel) { nodel->~balancedTree();}
		if (noder) { noder->~balancedTree();}
	}

	bool addNode(T node_)
	{
		int success = 0;

		if ( !occupied ) {
			value = node_;
			occupied = 1;

			return 1;
		}
		else if (value > node_) {
			if (!nodel) {
				nodel = new balancedTree<T>(this);
			}
			
			success = nodel->addNode(node_);

			if (noder) {
				depth = (nodel->depth > noder->depth ? nodel->depth : noder->depth) + 1;
			}
		
			else {
				depth = nodel->depth + 1;
			}
		}
		else if (value < node_) {
			if (!noder) {
				noder = new balancedTree<T>(this);
			}
			
			success = noder->addNode(node_);

			if (nodel) {
				depth = (nodel->depth > noder->depth ? nodel->depth : noder->depth) + 1;
			}

			else {
				depth = noder->depth + 1;
			}
		}
		else {
			return 0;
		}

		if (success) {
			int balancefactor = 0;
			if (!nodel && noder) {
				balancefactor = -noder->depth - 1;
			}
			else if (nodel && !noder) {
				balancefactor = nodel->depth + 1;
			}
			else if (nodel && noder) {
				balancefactor = nodel->depth - noder->depth;
			}

			change(balancefactor);
		}

		return success;
	}

	void change(int balancefactor_)
	{
		if (balancefactor_ == 2) {
			
			if (nodel->noder) {
				nodel->change(-2);
			}
			
			if (parent) {
				if (parent->nodel == this) {
					parent->nodel = nodel;
				}
				else {
					parent->noder = nodel;
				}
			}
			nodel->parent = parent;
			parent = nodel;
			nodel = NULL;
			
			parent->noder = this;

			parent->setDepth();
		}
		else if (balancefactor_ == -2) {

			if (noder->nodel) {
				noder->change(2);
			}

			if (parent) {
				if (parent->nodel == this) {
					parent->nodel = noder;
				}
				else {
					parent->noder = noder;
				}
			}
			noder->parent = parent;
			parent = noder;
			noder = NULL;

			parent->nodel = this;

			parent->setDepth();
		}
	}

	void getNodes()
	{
		std::cout << value << " ";

		if (nodel) { nodel->getNodes(); }
		if (noder) { noder->getNodes(); }
	}

	void getInfos()
	{
		if (nodel) { nodel->getInfos(); }
		if (noder) { noder->getInfos(); }

		if (!nodel && !noder) {
			std::cout << 0 << " " << depth << " " << value << std::endl;
		}
		else if (!nodel && noder) {
			std::cout << -noder->depth - 1 << " " << depth << " " << value << std::endl;
		}
		else if (nodel && !noder) {
			std::cout << nodel->depth + 1 << " " << depth << " " << value << std::endl;
		}
		else {
			std::cout << nodel->depth - noder->depth << " " << depth << " " << value << std::endl;
		}
	}

	void setDepth()
	{
		if (nodel) { nodel->setDepth(); }
		if (noder) { noder->setDepth(); }

		if (!nodel && !noder) {
			depth = 0;
		}
		else if (!nodel) {
			depth = noder->depth + 1;
		}
		else if (!noder) {
			depth = nodel->depth + 1;
		}
		else {
			depth = (nodel->depth > noder->depth ? nodel->depth : noder->depth) + 1;
		}
	}

	bool search(size_t value_)
	{
		if (value_ == value) { return 1; }

		if (value_ < value && nodel) { return nodel->search(value_); }
		if (value_ > value && noder) { return noder->search(value_); }

		return 0;
	}
};

}

#endif /* DATASTRUCTURES_H_ */