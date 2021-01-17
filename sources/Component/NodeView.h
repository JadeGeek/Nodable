#pragma once

#include "Nodable.h" // for constants and forward declarations
#include "View.h"    // base class
#include <imgui/imgui.h>   // for ImVec2
#include <string>
#include <map>
#include "Member.h"
#include <mirror.h>

namespace Nodable{

	/* We use this enum to identify all GUI detail modes */
	enum ViewDetail_
	{
		ViewDetail_Minimalist  = 0,
		ViewDetail_Essential   = 1,
		ViewDetail_Exhaustive  = 2,
		ViewDetail_Default     = ViewDetail_Essential
	};	

	class Node;

	/**
	 * Simple struct to store a member view state
	 */
	struct MemberView
    {
	    Member* member;

	    /** determine if input should be visible or not */
	    bool showInput;

	    /** false by default, will be true if user toggle showInput on/off */
	    bool touched;

	    /** Position in screen space */
	    ImVec2 screenPos;

	    explicit MemberView(Member* _member)
        {
	        member = _member;
            member = _member;
            showInput = true;
            touched = false;
        }
    };

	class NodeView : public View
	{
	private:
		/* Update function that takes a specific delta time (can be hacked by sending a custom value) */
		bool              update(float _deltaTime);

	public:
		NodeView();
		~NodeView();

		/** override method to extract some information from owner */
		void setOwner(Node* _node)override;

		/** Expose a member (connector and input if uncollapsed) */
		void exposeMember(Member* _member);

		/* Draw the view at its position into the current window
		   Returns true if nod has been edited, false either */
		bool              draw                ()override;

		/* Should be called once per frame to update the view */
		bool              update              ()override;		

		void updateInputConnectedNodes(Nodable::Node* node, float deltaTime);

		/* Get top-left corner vector position */
		ImVec2            getRoundedPosition         ()const;

		ImRect            getRect()const;

		/* Get the connector position of the specified member (by name) for its Way way (In or Out ONLY !) */
		ImVec2            getConnectorPosition(const Member *_member /*_name*/, Way /*_connection*/_way)const;

		/* Set a new position (top-left corner) vector to this view */ 
		void              setPosition         (ImVec2);

		/* Apply a translation vector to the view's position */
		void              translate           (ImVec2);

		/* Arrange input nodes recursively while keeping this node position unchanged */
		void              arrangeRecursively  ();

		/** Draw advance properties (components, dirty state, etc.) */
		void drawAdvancedProperties();
		
		/* Arrange input nodes recursively while keeping the nodeView at the position vector _position */
		static void       ArrangeRecursively  (NodeView* /*_nodeView*/);		

		/* Set the _nodeView as selected. 
		Only a single view can be selected at the same time */
		static void       SetSelected         (NodeView*);

		/* Return a pointer to the selected view or nullptr if no view are selected */
		static NodeView*  GetSelected         ();

		/* Return a pointer to the dragged member or nullptr if no member is dragged */
		static const Connector*  GetDraggedConnector() { return s_draggedConnector; }
		static void              ResetDraggedConnector() { s_draggedConnector = nullptr; }
		static void              StartDragConnector(const Connector* _connector) {
			if(s_draggedNode == nullptr)
				s_draggedConnector = _connector;
		};

		/* Return a pointer to the hovered member or nullptr if no member is dragged */
		static const Connector*  GetHoveredConnector() { return s_hoveredConnector; }

		/* Return true if _nodeView is selected */
		static bool       IsSelected(NodeView*);

		/* Set the _nodeView ad the current dragged view.
		Only a single view can be dragged at the same time. */
		static void       StartDragNode(NodeView*);

		static bool		  IsANodeDragged();

		static bool       IsInsideRect(NodeView*, ImRect);

		/* Move instantly _view to be inside _screenRect */
		static void       ConstraintToRect(NodeView*, ImRect);

		/* Return a pointer to the dragged view or nullptr if no view are dragged */
		static NodeView*  GetDragged          ();

        static bool DrawMemberInput(Member *_member, const char* _label = nullptr);

        static void DrawNodeViewAsPropertiesPanel(NodeView *_view);

        /** set globally the draw detail of nodes */
        static ViewDetail_ s_viewDetail;
    private:
		/*	Draw a Node Member at cursor position.
			Returns true if Member's value has been modified, false either */
		bool drawMemberView(MemberView &_memberView);
        void drawMemberConnectors(Member *_member);
		void drawConnector(ImVec2& , const Connector* , ImDrawList*);
        bool isMemberExposed(Member *_member);

		ImVec2          position;    // center position vector
		ImVec2          size;  // size of the window
		float           opacity; // global transparency of this view
		bool            collapsed;
		bool            pinned; // false: follow its outputs.
		float           borderRadius;
		ImColor         borderColorSelected;
		std::vector<MemberView> exposedMemberViews;

        /** pointer to the currently selected NodeView. */
		static NodeView* s_selected;
        /** pointer to the currently dragged NodeView. */
		static NodeView* s_draggedNode;
		static const Connector* s_draggedConnector;
		static const Connector* s_hoveredConnector;
		/** Minimum size for an input field */
        static const float s_memberInputSizeMin;
        /** Size of the small button to toggle input visibility on/off */
        static const ImVec2 s_memberInputToggleButtonSize;
        /** distance (on y axis) between two nodes */
        static const float s_nodeSpacingDistance;

        MIRROR_CLASS(NodeView)(
                MIRROR_PARENT(View));
        };
}
