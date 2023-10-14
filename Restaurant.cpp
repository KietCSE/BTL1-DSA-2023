#include "main.h"

extern int MAXSIZE;
class imp_res;

template<typename T>
struct Node {
	T data;
	int id;
	Node* next;
	Node* prev;
	Node() : data(T()), next(nullptr), prev(nullptr), id(0) {}; // luu y cho nay 
	Node(int id, const T& value, Node* p = nullptr, Node* ptr = nullptr) : data(value), next(p), prev(ptr), id(id) {};
	//Node(T&& value, Node* p = nullptr) : data(std::move(value)), Next(p) {};
};

// DESIGN QUEUE
template<typename T>
class Queue {
private:
	Node<T>* pHead;
	Node<T>* pTail;
	int count;

protected:
	// xoa 1 node tai 1 con tro
	

	// can dam bao end la mot node thuoc hang doi
	void IncreaseStep(Node<T>*& current, Node<T>* end, int step) {
		for (int i = 0; i < step && current != end; i++) current = current->next;
	}

	void DecreaseStep(Node<T>*& current, int step) {
		for (int i = 0; i < step && current != nullptr; i++) current = current->prev;
	}

	int SortSegment(Node<T>* start, Node<T>* end, int step, bool (*check)(T&, T&), bool (*equal)(T&, T&), void (*swap)(T&, T&)) {
		// current la node muon chen
		Node<T>* current = start;
		IncreaseStep(current, end, step);
		// hai con tro chay lui
		Node<T>* after = current;
		Node<T>* before = start;
		int Num = 0;
		while (current != end) {
			// selection
			while (after != start && ((check(before->data, after->data) && !equal(before->data, after->data)) 
				|| (equal(before->data, after->data) && after->id < before->id) ) ) {
				// swap
				/*T temp = after->data;
				after->data = before->data;
				before->data = temp;*/
				swap(after->data, before->data);
				int ID = after->id; 
				after->id = before->id; 
				before->id = ID;
				Num++;

				DecreaseStep(after, step);
				DecreaseStep(before, step);
			}
			IncreaseStep(current, end, step);
			after = current;
			before = current;
			DecreaseStep(before, step);
		}
		return Num;
	}

	Node<T>* FindMax(bool (*check)(T&, T&)) {
		Node<T>* ans = pHead;
		for (Node<T>* p = pHead; p != nullptr; p = p->next) {
			if (check(ans->data, p->data)) ans = p;
		}
		return ans;
	}

public:
	Queue() : pHead(nullptr), pTail(nullptr), count(0) {}

	~Queue() {
		Node<T>* current;
		while (this->pHead != nullptr) {
			current = pHead;
			pHead = pHead->next;
			delete current;
		}
		pHead = nullptr;
		pTail = nullptr;
		count = 0;
	}

	// them vao cuoi hang doi KHONG SU DUNG
	/*void Enqueue(const T& val) {
		if (this->count == MAXSIZE) return;
		if (pHead == nullptr) {
			pHead = new Node<T>(time, val);
			pTail = this->pHead;
		}
		else {
			pTail->next = new Node<T>(time, val, nullptr, pTail);
			pTail = pTail->next;
		}
		count++;
		time++;
	}*/

	void Enqueue(const T& val, int Time) {
		if (this->count == MAXSIZE) return;
		if (pHead == nullptr) {
			pHead = new Node<T>(Time, val);
			pTail = this->pHead;
		}
		else {
			pTail->next = new Node<T>(Time, val, nullptr, pTail);
			pTail = pTail->next;
		}
		count++;
	}

	// xoa o dau hang doi, khong delete nen truoc khi xoa can phai dung ham front
	void Dequeue() {
		if (pHead == nullptr) return;
		Node<T>* temp = pHead;
		pHead = pHead->next;
		delete temp;
		if (pHead) pHead->prev = nullptr;
		count--;
		if (count == 0) pTail = nullptr;
	}

	T& Front() {
		return this->pHead->data;
	}

	int IDfront() {
		return this->pHead->id;
	}

	void RemoveAt(Node<T>* current) {
		if (current == nullptr) return;
		if (current == pHead) {
			pHead = current->next;
			if (pHead) pHead->prev = nullptr;
			if (pHead == nullptr) pTail = nullptr;
			delete current;
		}
		else {
			current->prev->next = current->next;
			if (current->next) current->next->prev = current->prev;
			if (pTail == current) pTail = current->prev;
			delete current;
		}
		this->count--;
	}

	//int MaxTime(bool (*check)(T&)) {
	//	Node<T>* behind = pHead;
	//	Node<T>* front = pTail; 
	//	int maxtime = -1;
	//	while (behind->prev != front->next && front->next != behind) {
	//		if (check(behind->data) && behind->id > maxtime) maxtime = behind->id; 
	//		if (check(front->data) && front->id > maxtime) maxtime = front->id;
	//		front = front->prev; 
	//		behind = behind->next;
	//	}
	//	return maxtime;
	//}

	Node<T>* FindElementMaxTime(bool (*check)(T&)) {
		Node<T>* behind = pHead; 
		Node<T>* front = pTail;
		Node<T>* ans = nullptr;
		int ID = -1; 
		while (front != behind && front->next != behind) {
			if (check(behind->data) && behind->id > ID) { ID = behind->id; ans = behind; }
			if (check(front->data) && front->id > ID) { ID = front->id; ans = front; }
			front = front->prev;
			behind = behind->next;
		}
		if (front == behind) {
			if (check(front->data) && front->id > ID) ans = front;
		}
		return ans;
	}


	int ShellSort(bool (*check)(T&, T&), bool (*equal)(T&, T&), void (*swap)(T&, T&)) {
		int NumSwap = 0;
		// tim vi tri dung lai 
		Node<T>* end = FindMax(check); // vi sap xep theo abs va tim max abs nen ta dung chung 1 cmp
		if (end == nullptr) return -1; // truong hop hang doi rong

		// dem so phan tu can sap xep
		int cnt = 1; 
		Node<T>* a = pHead; 
		while (a != end) { cnt++; a = a->next; }

		end = end->next; 

		// duyet cac step
		for (int step = cnt / 2; step > 1; step /= 2) {
			Node<T>* p = this->pHead;
			// duyet cac day con
			for (int j = 0; j < step; j++) {
				NumSwap += SortSegment(p, end, step, check, equal, swap);
				p = p->next;
			}
		}
		NumSwap += SortSegment(pHead, end, 1, check, equal, swap);
		return NumSwap;
	}

	/*void Expansion2(bool (*check)(T&), void (*op)(T&)) {
		if (pHead == nullptr) return;
		// xet cac node o sau, tru node dau
		if (this->count > 1) {
			Node* current = pHead;
			while (current->next != nullptr) {
				Node* temp = current->next;
				if (check(temp->data)) {
					current->next = current->next->next;
					// giai phong va in con tro
					op(temp->data);
					delete temp;
					if (current->next) current->next->prev = current;
					this->count--;
				}
				else current = current->next;
			}
		}
		// xet den node dau
		if (check(pHead->data)) this->Dequeue();
		// cap nhap lai pTail
		if (this->pHead) {
			Node* p = this->pHead; 
			while (p->next) p = p->next; 
			this->pTail = p; 
		}
	}*/

	//void Expansion2(bool (*check)(T&), void (*op)(T&)) {
	//	if (pHead == nullptr) return;
	//	// xet cac node o sau, tru node dau
	//	if (this->count > 1) {
	//		Node<T>* current = pTail;
	//		while (current != nullptr) {
	//			if (check(current->data)) {
	//				Node<T>* temp = current; 
	//				current = current->prev; 
	//				op(temp->data); // in ra
	//				RemoveAt(temp);
	//			}
	//			else current = current->prev;
	//		}
	//	}
	//}


	//void Expansion(bool (*check)(T&), void (*op)(T&)) {
	//	if (pHead == nullptr) return; 
	//	Node<T>* move = pHead;
	//	Node<T>* position = nullptr; 
	//	while (move != nullptr) {
	//		// tim node < 0 dau tien+
	//		while (move != nullptr && !check(move->data)) move = move->next; 
	//		if (move != nullptr) {
	//			position = move; 
	//			// tim node co time lon nhat 
	//			for (move; move != nullptr; move = move->next) {
	//				if (check(move->data) && move->id > position->id) {
	//					position = move; 
	//				}
	//			}
	//			op(position->data); // in ra 
	//			RemoveAt(position); 
	//			move = pHead; 
	//		}
	//	}
	//	// khong can cap nhap lai pTail
	//}


	/*bool isExist(bool (imp_res::*check)(T&, string&), imp_res* obj, string& name) {
		for (Node* p = pHead; p != nullptr; p = p->next) {
			if (obj->check(p->data, name)) return true;
		}
		return false;
	}*/

	bool isExist(bool (*check)(T&, string&), string& name) {
		for (Node<T>* p = pHead; p != nullptr; p = p->next) {
			if (check(p->data, name)) return true;
		}
		return false;
	}

	void traverse(void (*op)(T&)) {
		for (Node<T>* p = pHead; p != nullptr; p = p->next) {
			op(p->data);
		}
	}


	int Size() { return this->count; }
	bool isEmpty() { return this->count == 0; }

	// ham nay dung de test
	void PrintQueue() {
		for (Node<T>* p = pHead; p != nullptr; p = p->next) {
			cout << p->data << ' ' << p->id << endl;;
		}
	}
	// ham dung de test
	void PrintQueue(void (*op)(T&)) {
		for (Node<T>* p = pHead; p != nullptr; p = p->next) {
			op(p->data);
		}
	}
};



// DESIGN CYCLE LINKLIST
class imp_res : public Restaurant {
private:
	Queue<customer*>* OrderOfTable = nullptr;
	Queue<customer*>* QueueCustomer = nullptr;
	customer* pCurrent; // nam giu vi tri X
	int TotalEnergyOfCTS;
	int TotalEnergyOfOL;
	int size;
	int time;

protected:
	void AddClockWise(customer* start, string& name, int& energy, int Time = -1) {
		customer* cus = new customer(name, energy, start, start->next);
		start->next->prev = cus;
		start->next = cus;
		customer* CusOrder = cus;

		if (Time == -1) OrderOfTable->Enqueue(CusOrder, this->time);
		else OrderOfTable->Enqueue(CusOrder, Time);

		this->size++;
		pCurrent = start->next; // cap nhap lai vi tri X
		if (Time == -1) {
			if (energy > 0) TotalEnergyOfCTS += energy;
			else TotalEnergyOfOL += energy;
		}
	}

	void AddNonClockWise(customer* start, string& name, int& energy, int Time = -1) {
		customer* cus = new customer(name, energy, start->prev, start);
		start->prev->next = cus;
		start->prev = cus;
		customer* CusOrder = cus;

		if (Time == -1) OrderOfTable->Enqueue(CusOrder, this->time);
		else OrderOfTable->Enqueue(CusOrder, Time);

		this->size++;
		pCurrent = start->prev; // cap nhap lai vi tri X
		if (Time == -1) {
			if (energy > 0) TotalEnergyOfCTS += energy;
			else TotalEnergyOfOL += energy;
		}
	}

	bool isExistNameInTable(string name) {
		if (pCurrent == nullptr) return false;
		customer* p = pCurrent;
		do {
			if (p->name == name) return true;
			p = p->next;
		} while (p != nullptr && p != pCurrent);
		return false;
	}

	// xoa het ban 
	void clearTable() {
		if (pCurrent) {
			customer* p = pCurrent->next;
			customer* temp = nullptr;
			while (p != nullptr && p != pCurrent) {
				temp = p;
				p = p->next;
				temp->next = nullptr;
				temp->prev = nullptr;
				delete temp;
			}
			pCurrent->next = nullptr;
			pCurrent->prev = nullptr;
			delete pCurrent;
			pCurrent = nullptr;
		}
		// can cho cac con tro tro den null truoc khi xoa 
		auto lamda2 = [](customer*& val) {val = nullptr; };
		OrderOfTable->traverse(lamda2);
		//delete OrderOfTable;
	}

	void Remove(customer* position) {
		if (pCurrent == nullptr) return;
		// cap nhap lai tong energy
		if (position->energy > 0) TotalEnergyOfCTS -= position->energy;
		else TotalEnergyOfOL -= position->energy;

		if (this->size == 1) {
			delete position; 
			pCurrent = nullptr;
			this->size--;
		}
		else if (this->size == 2) {
			pCurrent = position->next; 
			position->next = nullptr; 
			position->prev = nullptr; 
			delete position; 
			pCurrent->next = nullptr; 
			pCurrent->prev = nullptr; 
			this->size--; 
		}
		else {
			position->prev->next = position->next;
			position->next->prev = position->prev; 
			// cap nhap lai vi tri pCurrent 
			if (position->energy > 0) pCurrent = position->next;
			else pCurrent = position->prev; 
			position->next = nullptr; 
			position->prev = nullptr;
			delete position;
			this->size--;
		}
	}

	void PushFromQueue(string name, int energy, int Time)
	{
		if (energy == 0) return;

		if (this->pCurrent == nullptr) {
			customer* cus = new customer(name, energy, nullptr, nullptr);
			pCurrent = cus;
			customer* CusOrder = cus;
			OrderOfTable->Enqueue(CusOrder, Time); // truyen time vao
			this->size++;
			/*if (energy > 0) TotalEnergyOfCTS += energy;
			else TotalEnergyOfOL += energy;*/
		}
		// tao thanh vong tron
		else if (this->size == 1) {
			if (this->size == MAXSIZE) return;
			customer* cus = new customer(name, energy, pCurrent, pCurrent);
			pCurrent->prev = cus;
			pCurrent->next = cus;
			customer* CusOrder = cus;
			OrderOfTable->Enqueue(CusOrder, Time);
			this->size++;
			pCurrent = pCurrent->next; // cap nhap lai vi tri X
			/*if (energy > 0) TotalEnergyOfCTS += energy;
			else TotalEnergyOfOL += energy;*/
		}
		else if (this->size < MAXSIZE / 2) {
			// them cung chieu kim dong do
			if (energy >= pCurrent->energy) {
				this->AddClockWise(pCurrent, name, energy, Time);
			}
			// them nguoc chieu kim dong ho
			else {
				this->AddNonClockWise(pCurrent, name, energy, Time);
			}
		}
		else if (this->size >= MAXSIZE / 2 && this->size < MAXSIZE) {
			customer* position = pCurrent;
			customer* index = pCurrent;
			int RES = abs(position->energy - energy);

			// tim vi tri co RES lon nhat 
			do {
				if (abs(position->energy - energy) > RES) {
					index = position;
					RES = abs(position->energy - energy);
				}
				position = position->next;
			} while (position != pCurrent);

			// them vao ban an
			if (energy - index->energy > 0) {
				this->AddClockWise(index, name, energy, Time);
			}
			else {
				// them nguoc chieu kim dong ho
				this->AddNonClockWise(index, name, energy, Time);
			}
		}
	}

public:

	/*bool checkExistNameInQueue(customer*& a, string& name) {
		if (a->name == name) return true;
		else return false;
	}*/

	imp_res() {
		OrderOfTable = new Queue<customer*>;
		QueueCustomer = new Queue<customer*>;
		pCurrent = nullptr;
		TotalEnergyOfCTS = 0;
		TotalEnergyOfOL = 0;
		size = 0;
		time = 0;
	};

	~imp_res() {
		TotalEnergyOfCTS = 0;
		TotalEnergyOfOL = 0;
		size = 0;
		time = 0;
		this->clearTable();
		// xoa khach hang trong hang doi
		auto lamda1 = [](customer*& val) { delete val; };
		QueueCustomer->traverse(lamda1);
		delete QueueCustomer;
		delete OrderOfTable;
	};

	void RED(string name, int energy)
	{
		if (energy == 0) return;
		if (this->isExistNameInTable(name)) return;

		auto lamda = [](customer*& a, string& name) {return a->name == name; };
		if (QueueCustomer->isExist(lamda, name)) return;

		if (this->pCurrent == nullptr) {
			customer* cus = new customer(name, energy, nullptr, nullptr);
			pCurrent = cus;
			customer* CusOrder = cus;
			OrderOfTable->Enqueue(CusOrder, this->time); // truyen time vao
			this->size++;
			if (energy > 0) TotalEnergyOfCTS += energy;
			else TotalEnergyOfOL += energy;
		}
		// tao thanh vong tron
		else if (this->size == 1) {
			if (this->size == MAXSIZE) return;
			customer* cus = new customer(name, energy, pCurrent, pCurrent);
			pCurrent->prev = cus;
			pCurrent->next = cus;
			customer* CusOrder = cus;
			OrderOfTable->Enqueue(CusOrder, this->time);
			this->size++;
			pCurrent = pCurrent->next; // cap nhap lai vi tri X
			if (energy > 0) TotalEnergyOfCTS += energy;
			else TotalEnergyOfOL += energy;
		}
		else if (this->size < MAXSIZE / 2) {
			// them cung chieu kim dong do
			if (energy >= pCurrent->energy) {
				this->AddClockWise(pCurrent, name, energy);
			}
			// them nguoc chieu kim dong ho
			else {
				this->AddNonClockWise(pCurrent, name, energy);
			}
		}
		else if (this->size >= MAXSIZE / 2 && this->size < MAXSIZE) {
			customer* position = pCurrent;
			customer* index = pCurrent;
			int RES = abs(position->energy - energy);

			// tim vi tri co RES lon nhat 
			do {
				if (abs(position->energy - energy) > RES) {
					index = position;
					RES = abs(position->energy - energy);
				}
				position = position->next;
			} while (position != pCurrent);

			// them vao ban an
			if (energy - index->energy > 0) {
				this->AddClockWise(index, name, energy);
			}
			else {
				// them nguoc chieu kim dong ho
				this->AddNonClockWise(index, name, energy);
			}
		}
		// truong hop het ban thi day vao hang doi
		else {
			if (QueueCustomer->Size() < MAXSIZE) {
				if (energy > 0) TotalEnergyOfCTS += energy;
				else TotalEnergyOfOL += energy;
				customer* cus = new customer(name, energy, nullptr, nullptr);
				this->QueueCustomer->Enqueue(cus, this->time);
			}
		}
		this->time++;
	}


	void BLUE(int num) {
		if (num > this->size) this->clearTable(); 
		else {
			for (int i = 0; i < num; i++) {
				customer* p = OrderOfTable->Front();
				Remove(p); // da dam bao cap nhap tong energy va pCurrent 
				OrderOfTable->Front() = nullptr;
				OrderOfTable->Dequeue();
			}
		}
		while (!QueueCustomer->isEmpty() && this->size < MAXSIZE) {
			customer* p = QueueCustomer->Front();
			int id = QueueCustomer->IDfront();
			this->PushFromQueue(std::move(p->name), std::move(p->energy), id); // luu y cho nay 
			// xoa khoi hang doi 
			p->next = nullptr; 
			p->prev = nullptr; 
			delete p; 
			QueueCustomer->Dequeue();
		}
	}


	void PURPLE() {
		auto check = [](customer*& a, customer*& b)->bool {return abs(a->energy) <= abs(b->energy); };
		auto equal = [](customer*& a, customer*& b)->bool {return abs(a->energy) == abs(b->energy); };
		auto swap = [](customer*& a, customer*& b) ->void {
			string N = std::move(a->name);
			int E = std::move(a->energy);
			a->name = std::move(b->name);
			a->energy = std::move(b->energy);
			b->name = std::move(N);
			b->energy = std::move(E);
		};
		int NumSwap = QueueCustomer->ShellSort(check, equal, swap);
		NumSwap %= MAXSIZE;
		cout << NumSwap << endl;
		this->BLUE(NumSwap);
	}

	void REVERSAL() {
		if (this->size <= 1) return;
		// dao nguoc so duong
		string name = pCurrent->name; 
		customer* temp = pCurrent;

		customer* front = pCurrent->next;
		customer* behind = pCurrent;
		while (front != behind) {
			while (front->energy < 0 && front != behind) front = front->next; 
			while (behind->energy < 0 && behind != front) behind = behind->prev;
			if (behind == front) break; // truong hop khong co gi de dao nguoc

			string temp = std::move(front->name); 
			front->name = std::move(behind->name); 
			behind->name = std::move(temp);

			int e = std::move(front->energy); 
			front->energy = std::move(behind->energy); 
			behind->energy = std::move(e);

			front = front->next; 
			behind = behind->prev;
			if (front == behind->next) break;
		}
		// dao nguoc so am 
		front = pCurrent->next;
		behind = pCurrent;
		while (front != behind) {
			while (front->energy > 0 && front != behind) front = front->next;
			while (behind->energy > 0 && behind != front) behind = behind->prev;
			if (behind == front) break; // truong hop khong co gi de dao nguoc

			string temp = std::move(front->name);
			front->name = std::move(behind->name);
			behind->name = std::move(temp);

			int e = std::move(front->energy);
			front->energy = std::move(behind->energy);
			behind->energy = std::move(e);

			front = front->next;
			behind = behind->prev;
			if (front == behind->next) break;
		}

		while (temp->name != name) {
			temp = temp->next; 
		}
		pCurrent = temp;
	}


	/*void UNLIMITED_VOID() {
		if (this->size < 4) return; 
		if (this->size == 4) {
			//this->LIGHT(); 
		}
		else {
			// khoi tao cho chuoi 4 ki tu dau 
			int TotalFront = pCurrent->energy; 
			// luu vi tri dau cuoi
			customer* front = pCurrent; 
			customer* behind = pCurrent; 
			for (int i = 0; i < 3; i++) {
				front = front->next;
				TotalFront += front->energy;
			}

			int TotalBehind = 0; 
			int Max = -1000000000;
			int Ans = TotalFront;
			int len = 4;
			customer* I = front->next; 
			customer* J = pCurrent;
			customer* idxMax = pCurrent;
			int j = 0, jj = 0; 
						
			for (int i = 4; i < this->size; i++) {
				TotalBehind += J->energy; 
				
				if (TotalBehind > Max) {
					idxMax = J; 
					Max = TotalBehind;
					jj = j;
				}

				TotalFront += I->energy; 
				j++;
				J = J->next; 

				if (TotalFront <= Ans && TotalFront <= TotalFront - Max) {
					Ans = TotalFront;
					behind = pCurrent;
					front = I;
					len = i + 1;
				}
				else if (TotalFront - Max < Ans && TotalFront - Max < TotalFront) {
					Ans = TotalFront - Max; 
					behind = idxMax->next;
					front = I;
					len = i - jj;
				}
				else if (TotalFront - Max == Ans && len <= i - jj) {
					behind = idxMax->next;
					front = I;
					len = i - jj;
				}

				I = I->next;
			}

			cout << "Tong la: " << Ans << endl;
			for (customer* p = behind; p != front->next; p = p->next) {
				p->print();
			}
		}
	} */

	void UNLIMITED_VOID() {
		if (this->size < 4) return; 
		if (this->size == 4) {
			customer* p = pCurrent->prev; 
			do {
				p->print(); 
				p = p->next; 
			} while (p != pCurrent->prev);
		}
		else {
			// current la node bat dau cua day con
			customer* current = pCurrent;
			int AnsSum = 1000000000;
			customer* start = nullptr; 
			customer* end = nullptr;

			for (int i = 0; i < this->size; i++) {
				
				int Min = 100000000;
				int total = 0;
				customer* move = current; 
				customer* idxmin = current; 

				for (int i = 0; i < 3; i++) idxmin = idxmin->next; // luu vi tri ket thuc
				
				for (int j = 0; j < this->size; j++) {
					total += move->energy; 
					if (j >= 3 && total <= Min) {
						Min = total; 
						idxmin = move;
					}
					move = move->next;
				}

				if (Min <= AnsSum) {
					AnsSum = Min; 
					start = current; 
					end = idxmin;
				}
				
				current = current->next;
			}
			// IN RA 
			customer* st = start;
			for (customer* it = start; true; it = it->next) {
				if (st->energy > it->energy) st = it; 
				if (st == end) break; 
			}
			
			for (customer* it = st; true; it = it->next) {
				it->print(); 
				if (it == end) break; 
			}

			for (customer* it = start; st != start; it = it->next) {
				it->print(); 
				if (it->next == st) break;
			}

		}
	}

	void DOMAIN_EXPANSION() {
		if (TotalEnergyOfCTS >= abs(TotalEnergyOfOL)) {
			auto isOL = [](customer*& val)->bool { return val->energy < 0; };

			Node<customer*>* table = OrderOfTable->FindElementMaxTime(isOL);
			Node<customer*>* queue = QueueCustomer->FindElementMaxTime(isOL); 

			while (table != nullptr || queue != nullptr) {
				if (table == nullptr) {
					queue->data->print();
					queue->data->next = nullptr;
					queue->data->prev = nullptr; 
					delete queue->data; 
					QueueCustomer->RemoveAt(queue);
					queue = QueueCustomer->FindElementMaxTime(isOL);
				}
				else if (queue == nullptr) { 
					table->data->print();
					this->Remove(table->data); 
					table->data = nullptr; 
					OrderOfTable->RemoveAt(table);
					table = OrderOfTable->FindElementMaxTime(isOL);
				}
				else if (table->id > queue->id) {
					table->data->print();
					this->Remove(table->data);
					table->data = nullptr;
					OrderOfTable->RemoveAt(table);
					table = OrderOfTable->FindElementMaxTime(isOL);
					queue = QueueCustomer->FindElementMaxTime(isOL);
				}
				else {
					queue->data->print();
					queue->data->next = nullptr;
					queue->data->prev = nullptr;
					delete queue->data;
					QueueCustomer->RemoveAt(queue);
					table = OrderOfTable->FindElementMaxTime(isOL);
					queue = QueueCustomer->FindElementMaxTime(isOL);
				}
			}
		}
		else {
			auto isCTS = [](customer*& val)->bool { return val->energy > 0; };

			Node<customer*>* table = OrderOfTable->FindElementMaxTime(isCTS);
			Node<customer*>* queue = QueueCustomer->FindElementMaxTime(isCTS);

			while (table != nullptr || queue != nullptr) {
				if (table == nullptr) {
					queue->data->print();
					queue->data->next = nullptr;
					queue->data->prev = nullptr;
					delete queue->data;
					QueueCustomer->RemoveAt(queue);
					queue = QueueCustomer->FindElementMaxTime(isCTS);
				}
				else if (queue == nullptr) {
					table->data->print();
					this->Remove(table->data);
					table->data = nullptr;
					OrderOfTable->RemoveAt(table);
					table = OrderOfTable->FindElementMaxTime(isCTS);
				}
				else if (table->id > queue->id) {
					table->data->print();
					this->Remove(table->data);
					table->data = nullptr;
					OrderOfTable->RemoveAt(table);
					table = OrderOfTable->FindElementMaxTime(isCTS);
					queue = QueueCustomer->FindElementMaxTime(isCTS);
				}
				else {
					queue->data->print();
					queue->data->next = nullptr;
					queue->data->prev = nullptr;
					delete queue->data;
					QueueCustomer->RemoveAt(queue);
					table = OrderOfTable->FindElementMaxTime(isCTS);
					queue = QueueCustomer->FindElementMaxTime(isCTS);
				}
			}
		}
		while (!QueueCustomer->isEmpty() && this->size < MAXSIZE) {
			customer* p = QueueCustomer->Front();
			int id = QueueCustomer->IDfront();
			this->PushFromQueue(std::move(p->name), std::move(p->energy), id); // luu y cho nay 
			// xoa khoi hang doi 
			p->next = nullptr;
			p->prev = nullptr;
			delete p;
			QueueCustomer->Dequeue();
		}
	}


	//void DOMAIN_EXPANSION() {
	//	if (TotalEnergyOfCTS >= abs(TotalEnergyOfOL)) {
	//		auto isOL = [](customer*& val)->bool { return val->energy < 0; };
	//		
	//		auto lamda2 = [](customer*& val)->void { val->print(); val->next = nullptr; val->prev = nullptr; delete val; };
	//		// giai phong o nho trong hang doi customer
	//		QueueCustomer->Expansion(isOL, lamda2);

	//		auto lamda1 = [](customer*& val)->void { val->print(); val = nullptr; };
	//		// dua tat ca customer den OL ve null va xoa khoi hang doi
	//		OrderOfTable->Expansion2(isOL, lamda1);

	//		customer* p = pCurrent; // moi lan xoa thi con tro pCurrent se thay doi
	//		customer* temp = nullptr;
	//		int n = this->size;
	//		for (int i = 0; i < n; i++) {
	//			temp = p; 
	//			p = p->next; 
	//			if (temp->energy < 0) Remove(temp);
	//		}
	//		
	//		TotalEnergyOfOL = 0;
	//	}
	//	else {
	//		auto isCTS = [](customer*& val)->bool {return val->energy > 0; };

	//		auto lamda2 = [](customer*& val)->void { val->print(); val->next = nullptr; val->prev = nullptr; delete val; };
	//		// giai phong o nho trong hang doi customer
	//		QueueCustomer->Expansion(isCTS, lamda2);

	//		auto lamda1 = [](customer*& val)->void { val->print(); val = nullptr; };
	//		// dua tat ca customer den OL ve null va xoa khoi hang doi
	//		OrderOfTable->Expansion2(isCTS, lamda1);

	//		customer* p = pCurrent; // moi lan xoa thi con tro pCurrent se thay doi
	//		customer* temp = nullptr;
	//		int n = this->size;
	//		for (int i = 0; i < n; i++) {
	//			temp = p;
	//			p = p->next;
	//			if (temp->energy < 0) Remove(temp);
	//		}
	//		TotalEnergyOfCTS = 0;
	//	}
	//	
	//	while (!QueueCustomer->isEmpty() && this->size < MAXSIZE) {
	//		customer* p = QueueCustomer->Front();
	//		this->RED(std::move(p->name), std::move(p->energy));
	//		p->next = nullptr;
	//		p->prev = nullptr;
	//		delete p;
	//		QueueCustomer->Dequeue();
	//	}
	//}


	void LIGHT(int num)
	{
		if (pCurrent == nullptr) return;
		if (num > 0) {
			customer* p = pCurrent; 
			do {
				p->print(); 
				p = p->next; 
			} while (p != pCurrent);
		}
		else if (num < 0) {
			customer* p = pCurrent;
			do {
				p->print();
				p = p->prev;
			} while (p != pCurrent);
		}
		else {
			auto lamda = [](customer*& val) { val->print(); };
			QueueCustomer->traverse(lamda);
		}

		cout << "size of table: " << this->size << endl;
		cout << "size of queue: " << QueueCustomer->Size() << endl;
		cout << "size of order: " << OrderOfTable->Size() << endl;
		
		/*customer* p = pCurrent;
		do {
			p->print();
			p = p->next;
		} while (p != pCurrent && p != nullptr);
		cout << "size of table: " << this->size << endl;
		cout << "size of queue: " << QueueCustomer->Size() << endl;
		cout << "size of order: " << OrderOfTable->Size() << endl;
		cout << "Total CTS: " << TotalEnergyOfCTS << endl;
		cout << "Total OL: " << TotalEnergyOfOL << endl;*/
		

		/*auto lamda = [](customer*& a) { a->print(); };
		QueueCustomer->PrintQueue(lamda);
		cout << "-------" << endl;*/
		/*this->Remove(pCurrent);

		customer* ptr = pCurrent;
		do {
			ptr->print();
			ptr = ptr->next;
		} while (ptr != pCurrent && ptr != nullptr);*/

		//customer* a = QueueCustomer->Front();
		//customer* b = new customer(a->name, a->energy, nullptr, nullptr); // copy su lieu
		//a->print(); 
		//b->print();
		
		/*QueueCustomer->Front()->next = nullptr; 
		QueueCustomer->Front()->prev = nullptr;
		delete QueueCustomer->Front();
		QueueCustomer->Dequeue();
		cout << QueueCustomer->Size() << endl;

		QueueCustomer->Front()->next = nullptr;
		QueueCustomer->Front()->prev = nullptr;
		delete QueueCustomer->Front();
		QueueCustomer->Dequeue();
		cout << QueueCustomer->Size() << endl;*/
		//delete b;
	}
};