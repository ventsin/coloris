#pragma once

#include <nana/gui.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/scroll.hpp>

namespace nana
{
	/** Panel specialization to display small visible scrollable part of the panel
		The scrolled panel is large and mostly hidden.  The widgets are placed on this panel.
		The visible panel is small and transparent, allows user to see a small part of the scrolled panel.
		The visible panel is surrounded by scroll bars which control what part of scrolled panel is seen.
		The visible panel and scroll bars are constructed automatically, to the dimensions specifed in the scrolled panel constructor
	 */

	class scrollpanel : public nana::panel<false>
	{
	public:

		/** CTOR
			@param[in] parent window
			@param[in] visible_panel size of visible window, including scroll bars
			@param[in] scrolled_panel size of area over which window moves
		The window parent/child arrangement:
		<pre>
		parent
			<----- myPanelAndScrolls
							  <----------Visible
										<----------------BASE ( scrolled panel )
							  <----------Vertical Scroll
							  <--------- Horizontal Scroll
		</pre>
		*/
		scrollpanel() {}

		scrollpanel(
			const nana::window& parent,
			const nana::rectangle& visible_panel,
			const nana::rectangle& scrolled_panel)
		{
			create(parent, visible_panel, scrolled_panel);
		}
		
		void create(const nana::window& parent, const nana::rectangle& visible_panel, const nana::rectangle& scrolled_panel)
		{
			myVisibleAndScrolls.create(parent, true);
			myVisible.create(myVisibleAndScrolls, true);
			myScrollVert.create(myVisibleAndScrolls, true);
			myPlace.bind(myVisibleAndScrolls);

			myScrollVert.size({15, myScrollVert.size().height});
			myScrollVert.step(20);
			myScrollVert.range(100);

			// create base panel over which scrollable window moves
			panel<false>::create(myVisible, true);
			panel<false>::move(scrolled_panel);

			// place scrolling window and scrolls in position on parent
			myVisibleAndScrolls.move(visible_panel);

			// Set scroll ammounts to the full extent of the scrolled area
			scroll_amount();

			// register scroll event handlers
			myScrollVert.events().value_changed([&]
				{
					panel<false>::move(
						0,
						-(int)myScrollVert.value());
				});

			// arrange visible panel with scrolls at right and bottom
			myPlace.div("<vert <<panel><scroll_vert weight=16>>>");
			myPlace["panel"] << myVisible;
			myPlace["scroll_vert"] << myScrollVert;
			myPlace.collocate();
		}

		/// Change size of scrolled panel
		void size(const nana::size & s)
		{
			panel<false>::size(s);
			scroll_amount();
		}

		void move(const rectangle & r)
		{
			myVisibleAndScrolls.move(r);
		}

	private:
		nana::panel<false> myVisibleAndScrolls;   /// Window showing scrolling window and scrolls
		nana::panel<false> myVisible;           ///< Part of scrolled panel that is visible
		nana::scroll<true> myScrollVert;
		nana::place myPlace;

		/// Adjust scrolling amounts
		void scroll_amount()
		{
			//auto h = panel<false>::size().height;
			//auto h2 = myVisible.size().height;
			int y = panel<false>::size().height - myVisible.size().height;
			if (y < 0)
				y = 0;
			myScrollVert.amount(y);
		}
	};
}