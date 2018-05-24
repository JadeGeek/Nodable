#pragma once

#include "Nodable.h"     // forward declarations
#include "Component.h"   // base class
#include <string>
#include <vector>

namespace Nodable{
	/* Class Container is a factory able to create all kind of Node 
	   All Symbol nodes's pointers created within this context are referenced in a vector to be found later */
	class Container: public Component{
	public:
		DECLARE_COMPONENT(Container);

		virtual ~Container();
		void                      draw();
		void                      clear();
		void                      frameAll();
		void                      drawLabelOnly();
		size_t                    getSize()const;
		Variable* 	          		find                      (std::string /*Symbol name*/);
		void                      addNode                   (Entity* /*Node to add to this context*/);
		void                      destroyNode               (Entity*);
		Variable*					createNodeVariable        (std::string /*name*/ = "");
		Variable*					createNodeNumber          (double /*value*/ = 0);
		Variable*					createNodeNumber          (const char* /*value*/);
		Variable*					createNodeString          (const char* /*value*/);
		Entity*                   createNodeBinaryOperation (std::string /*_operator*/);
		Entity*                   createNodeAdd             ();
		Entity*                   createNodeSubstract       ();
		Entity*			          createNodeMultiply        ();
		Entity*			          createNodeDivide          ();
		Entity*			          createNodeAssign          (); 
		Lexer*                    createNodeLexer           (Variable* /*expression*/);
		std::vector<Variable*>& getVariables(){return variables;}
	private:
		std::vector<Variable*> variables; /* Contain all Symbol Nodes created by this context */
		std::vector<Entity*>        entities;   /* Contain all Objects created by this context */
		Entity*                     parent;
	};
}