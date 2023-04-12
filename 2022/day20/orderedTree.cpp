//[UTF-8 en/ru]
#include <orderedTree.hpp>
	
cTree:T& find(int index){
	if(index <= 0 || size <= index){
		//exception
	}
	// unwrap to stack
	//exception чтото пошло не так.
}
	
cTree:void remove(int index){
		// сейчас достаточно просто удалить ссылку на элемент, 
		// в инсерте этот же элемент появится обратно
		// иначе - через new/delete?

	cTree* ptr = &find(index);
	cTree* tmp = ptr->left;
	if(tmp != null)
	for(;tmp->right != null; tmp = tmp->right){
		
	}
	//swap to leaf
	for(; next != nullptr; next = chooseChild(ptr)){
		ptr->element = move(next->element);
		ptr = next;
	}
	//delete leaf
	cTree* prev = ptr;
	delete ptr;

	//go back - restore sizes
	ptr->size -= 1;
	for(; prev != nullptr; prev = ptr->parnet){
		//restore sizes;
		ptr = prev;
		if(abs(ptr->left->size + ptr->right->size) >=2){
			// если такое бывает - в чем я не уверен пока что
			// стоит задуматься над балансировкой.
		}
		ptr->size = 1 + ptr->left->size + ptr->right->size;
	}
}


cTree:void insert(int index){
	// либо формирую дерево в конструкторе из массива
	// либо только что удаленый элемент переставляю в другое место
	// иначе - через new/delete?

	cTree* ptr = &find(index);
	cTree* next = chooseChild(ptr);

	//swap to leaf
	for(; next != nullptr; next = chooseChild2(ptr)){ // выбрать пустое или где меньше.
		ptr->element = move(next->element);
		ptr = next;
	}
	//delete leaf
	cBinaryTree* prev = ptr;
	delete ptr;

	//go back - restore sizes
	ptr->size -= 1;
	for(; prev != nullptr; prev = ptr->parnet){
		//restore sizes;
		ptr = prev;
		if(abs(ptr->left->size + ptr->right->size) >=2){
			// если такое бывает - в чем я не уверен пока что
			// стоит задуматься над балансировкой.
		}
		ptr->size = 1 + ptr->left->size + ptr->right->size;
	}
			
}