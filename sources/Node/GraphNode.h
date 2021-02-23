#pragma once

#include <string>
#include <vector>
#include <imgui/imgui.h>   // for ImVec2
#include <mirror.h>
#include <Node/AbstractCodeBlockNode.h>

#include "Nodable.h"
#include "Component.h"
#include "Node.h"
#include "Language/Common/Language.h"

namespace Nodable{

    // forward declaration
    class ScopedCodeBlockNode;
    class InstructionNode;
    class CodeBlockNode;

    /**
     * @brief
     * The role of a GraphNode is to manage a set of Node and Wire used in a ScopedCodeBlockNode with a given language.
     *
     * @details
     * Nodes and Wires are instantiated and destroyed by this class.
     * The ScopedCodeBlockNode contain the structure of the program in which Nodes are used.
     */
	class GraphNode: public Node {
	public:

		explicit GraphNode(const Language* _language);
		~GraphNode() override;

		/** Update the graph by evaluating its nodes only when necessary. */
        UpdateResult                update() override;

        /** Clear Graph. Delete all Nodes/Wires and reset scope */
		void                        clear();

		/** Find a VariableNode given its name/identifier
		 * @return - a valid VariableNode* or nullptr if variable is not found. */
		VariableNode*               findVariable(std::string);

		inline std::vector<Node*>&  getNodeRegistry() {return nodeRegistry;}
		inline std::vector<Wire*>&  getWireRegistry() {return wireRegistry;}

        [[nodiscard]] inline const Language*      getLanguage()const { return language; }
        [[nodiscard]] inline ScopedCodeBlockNode* getScope(){ return scope;}
        [[nodiscard]] bool                        hasInstructionNodes();

		/** This will arrange all NodeViews after a Graph modification. */
        void                        arrangeNodeViews();

		/* node factory */
		CodeBlockNode*              newCodeBlock();
        InstructionNode*		    newInstruction(CodeBlockNode* _parentCodeBlock);
        InstructionNode*            newInstruction();
		VariableNode*				newVariable(std::string, ScopedCodeBlockNode*);
		VariableNode*				newNumber(double = 0);
		VariableNode*				newNumber(const char*);
		VariableNode*				newString(const char*);
		Node*                       newBinOp(const Operator*);
		Node*                       newUnaryOp(const Operator*);
        Node*                       newOperator(const Operator*);
		Wire*                       newWire();
		Node*                       newFunction(const Function* _proto);

        /** Connects two Member using a Wire (oriented edge)
         *  If _from is not owned, _to will digest it and nullptr is return.
          * Otherwise a new Wire will be created ( _from -----> _to) and returned.
          */
        Wire* connect(Member *_source, Member *_target);

        /** Disconnects a wire. This method is the opposite of Node::Connect.*/
        void disconnect(Wire* _wire);

    private:
        void registerNode(Node* node);
        void unregisterNode(Node* node);
        void deleteNode(Node* _node);

        void registerWire(Wire *_wire);
        void unregisterWire(Wire *_wire);
        /*   deleteWire is disconnect(Wire*) */

	private:		
		std::vector<Node*> nodeRegistry;
		std::vector<Wire*> wireRegistry;
		const Language* language;
		ScopedCodeBlockNode* scope;
	public:
		static ImVec2               LastResultNodeViewPosition;

		/** reflect class with mirror */
		MIRROR_CLASS(GraphNode)
		(
			MIRROR_PARENT(Node) // we only need to know parent
        )
    };
}