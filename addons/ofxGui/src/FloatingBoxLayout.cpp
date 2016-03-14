#include "FloatingBoxLayout.h"

namespace ofx {
namespace DOM {

FloatingBoxLayout::FloatingBoxLayout(Element* parent, Orientation orientation):
	BoxLayout(parent, orientation){
}

FloatingBoxLayout::~FloatingBoxLayout(){
}

void FloatingBoxLayout::processConfig(const ofJson & config){
}

void FloatingBoxLayout::doLayout(){

	if (_parent && !_isDoingLayout){
		// Prevent recursive calls to doLayout.
		_isDoingLayout = true;

		float totalWidth = 0;
		float totalHeight = 0;
		float currentX = 0;
		float currentY = 0;

		for (Element* element: children()){

			if (element){

				if(!element->isHidden()){
					// TODO ignoring Orientation right now
					// because it is not clear if this is device orientation (my guess)
					// or horizontal vs. vertical alignment of elements (which the code implied)

					if(element->hasAttribute("float")){

						LayoutFloat layoutfloat = element->getAttribute<LayoutFloat>("float");

						switch(layoutfloat){

							case LayoutFloat::LEFT: {

								setNextLeftFloatPosition(ofPoint(0,0), element);


								break;
							}

							case LayoutFloat::RIGHT: {

								// TODO
								setNextRightFloatPosition(ofPoint(_parent->getWidth()-element->getWidth(),0), element);

								break;
							}

							case LayoutFloat::NONE:
							default:{

								element->setPosition(currentX, currentY);

								break;
							}

						}

					}

					totalWidth = std::max(totalWidth, element->getShape().getRight());
					currentY = std::max(currentY, element->getShape().getBottom());
					totalHeight = std::max(totalHeight, element->getShape().getBottom());


				}
			}
		}

		totalWidth = std::max(totalWidth, _parent->getShape().getWidth());
		_parent->setSize(totalWidth, totalHeight);

		_isDoingLayout = false;
	}

}

///\brief floating layout based on https://www.w3.org/TR/CSS2/visuren.html#float-rules
void FloatingBoxLayout::setNextLeftFloatPosition(ofPoint p, Element* e){

	nextLeftFloatPositionLoop(e, p);

	e->setPosition(p);

}

void FloatingBoxLayout::nextLeftFloatPositionLoop(Element* e, ofPoint& res){

	for (Element* sibling: children()){

		if(sibling == e){
			return;
		}

		if(sibling){

			if(!sibling->isHidden()){

				float s_top = sibling->getShape().getTop();
				float s_bottom = sibling->getShape().getBottom();
				float s_right = sibling->getShape().getRight();
				float s_left = sibling->getShape().getLeft();

				if(sibling->hasAttribute("float")){

					LayoutFloat lf = sibling->getAttribute<LayoutFloat>("float");

					switch(lf){

						case LayoutFloat::NONE:
						default:
						case LayoutFloat::LEFT: {

							//rule 2, rule 7, rule 3
							if((res.x < s_right) && (res.y < s_bottom)){

								//trying to align element on the right side
								//check if there is a right floating sibling in the way
								ofPoint alignright = res;
								alignright.x = s_right;

								if(s_right + e->getWidth() < _parent->getWidth()
										&& worksWithFloats(alignright, e, LayoutFloat::RIGHT)){
									res.x = s_right;
									nextLeftFloatPositionLoop(e, res);
								}
								else {
									//does not fit to the right, move to next most left place
									res.y = s_bottom;
									res.x = 0;
									nextLeftFloatPositionLoop(e, res);
								}
							}
							break;
						}

						case LayoutFloat::RIGHT: {

							//rule 3
							if(res.x == 0 && s_left < res.x + e->getWidth() && res.y < s_bottom){
								//does not fit next to right floating sibling, move to the bottom of sibling
								res.y = s_bottom;
							}

							break;
						}

					}

				}

				//rule 5, rule 6
				if(res.y < s_top){
					res.y = s_top;
					nextLeftFloatPositionLoop(e, res);
				}

			}
		}
	}
}

void FloatingBoxLayout::setNextRightFloatPosition(ofPoint p, Element* e){

	nextRightFloatPositionLoop(e, p);

	e->setPosition(p);

}

void FloatingBoxLayout::nextRightFloatPositionLoop(Element* e, ofPoint& res){

	for (Element* sibling: children()){

		if(sibling == e){
			return;
		}

		if(sibling){

			if(!sibling->isHidden()){

				float e_width = e->getWidth();
				float start_x = _parent->getWidth()-e_width;
				float s_top = sibling->getShape().getTop();
				float s_bottom = sibling->getShape().getBottom();
				float s_right = sibling->getShape().getRight();
				float s_left = sibling->getShape().getLeft();

				if(sibling->hasAttribute("float")){

					LayoutFloat lf = sibling->getAttribute<LayoutFloat>("float");

					switch(lf){

						case LayoutFloat::RIGHT: {

							//rule 2, rule 7, rule 3
							if((res.x+e_width > s_left) && (res.y < s_bottom)){

								//trying to align element on the right side
								//check if there is a right floating sibling in the way
								ofPoint alignright = res;
								alignright.x = s_left-e_width;

								if(alignright.x >= 0
										&& worksWithFloats(alignright, e, LayoutFloat::LEFT)
										&& worksWithFloats(alignright, e, LayoutFloat::NONE)){
									res.x = alignright.x;
									nextRightFloatPositionLoop(e, res);
								}
								else {
									//does not fit to the right, move to next most left place
									res.y = s_bottom;
									res.x = start_x;
									nextRightFloatPositionLoop(e, res);
								}
							}
							break;
						}

						case LayoutFloat::NONE:
						default:
						case LayoutFloat::LEFT: {

							//rule 3
							if(res.x == start_x && s_right > res.x && res.y < s_bottom){
								//does not fit next to left floating sibling, move to the bottom of sibling
								res.y = s_bottom;
							}

							break;
						}

					}

				}

				//rule 5, rule 6
				if(res.y < s_top){
					res.y = s_top;
					nextRightFloatPositionLoop(e, res);
				}

			}

		}
	}
}

bool FloatingBoxLayout::worksWithFloats(ofPoint pos, Element* e, LayoutFloat floatType){

	for (Element* sibling : children()){

		if(sibling == e){
			return true;
		}

		if(sibling){

			if(!sibling->isHidden()){
				if(sibling->hasAttribute("float")){

					if(sibling->getAttribute<LayoutFloat>("float") == floatType){

						ofRectangle test(pos.x, pos.y, e->getWidth(), e->getHeight());
						if(sibling->getShape().intersects(test)){
							return false;
						}

					}
				}
			}

		}
	}
	return true;
}


}} // namespace ofx::DOM
