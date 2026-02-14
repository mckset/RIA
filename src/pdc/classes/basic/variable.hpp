/*
	variable.hpp
	The Var gives allows the engine to expand it's variables and classes without recompiling.
	Instead of adding a new class to the engine, the Var class is used to store the values that would be used in that class.
	It's mostly there to save values with a name to a file rather hard coding them in the game files.
	A new Var must be initialized with Assign().
	Though it can be created manually, Assign() provides an easy and memory safe way to store a value.
	Don't forget to free the variable when done.
	GetValue<type>() and GetPointer<type>() act as a way to easily work with a Var instead of constantly having to typecast.

	Keep in mind, void* is a very powerful and dangerous tool.
	Do not use this class unless it is absolutely necessary.
*/

#include <typeinfo>

class Var{
	public:
		string name = "";
		string type = "";
		void* object = nullptr;

		template <typename Type> void Assign(Type obj){
			if (object)
				free(object);
			Type* o = new Type();
			*o = obj;
			object = o;
			type = typeid(obj).name();
		}

		void Free(){
			if (object)
				free(object);
			name = "";
			type = "";
		}

		template <typename Type> Type GetValue(){
			return *(Type*)object;
		}

		template <typename Type> Type* GetPointer(){
			return (Type*)object;
		}

		template <typename Type> bool Is(){
			return typeid(Type).name() == type;
		}
};

void FreeVariableList(vector<Var>* vars){
	for (int i = 0; i < vars->size(); i++){
		if ((*vars)[i].Is<vector<Var>>())
			FreeVariableList((vector<Var>*)((*vars)[i]).object);
		free((*vars)[i].object);
	}
}