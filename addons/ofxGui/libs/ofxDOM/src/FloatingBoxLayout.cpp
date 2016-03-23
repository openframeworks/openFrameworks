#include "FloatingBoxLayout.h"

namespace ofx {
namespace DOM {

FloatingBoxLayout::FloatingBoxLayout(Element* parent, Orientation orientation):
	BoxLayout(parent, orientation){
}

FloatingBoxLayout::~FloatingBoxLayout(){
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

		if(!_parent->usesPercentalWidth()){
			totalWidth = std::max(totalWidth, _parent->getShape().getWidth());
			_parent->setWidth(totalWidth);
		}
		_parent->setHeight(totalHeight);

		_isDoingLayout = false;
	}

}

///\brief floating layout based on https://www.w3.org/TR/CSS2/visuren.html#float-rules
void FloatingBoxLayout::setNextLeftFloatPosition(ofPoint p, Element* e){

	p = nextLeftFloatPositionLoop(e, e, p);
	e->setPosition(p);

}

ofPoint FloatingBoxLayout::nextLeftFloatPositionLoop(Element* e, Element* s, const ofPoint& pos){

	ofPoint res = pos;

	for (Element* sibling: children()){

		if(sibling == s){
			return res;
		}

		if(sibling){

			if(!sibling->isHidden()){

				float s_top = sibling->getShape().getTop();
				float s_bottom = sibling->getShape().getBottom();
				float s_right = sibling->getShape().getRight();
				float s_left = sibling->getShape().getLeft();

				//rule 5, rule 6
				if(res.y < s_top){
					res.y = s_top;
					res.x = 0;
					res = nextLeftFloatPositionLoop(e, sibling, res);
				}

				if(sibling->hasAttribute("float")){

					LayoutFloat lf = sibling->getAttribute<LayoutFloat>("float");

					switch(lf){

						case LayoutFloat::NONE:
						default:
						case LayoutFloat::LEFT: {

							//rule 2, rule 7, rule 3
							if((res.x < s_right) && (res.y < s_bottom)){

								//trying to align element on the right side of the sibling

								ofPoint moveRight(s_right, res.y);
								ofPoint moveDown(0, s_bottom);

								if(s_right + e->getWidth() < _parent->getWidth()){
									moveRight = nextLeftFloatPositionLoop(e, sibling, moveRight);
									moveDown = nextLeftFloatPositionLoop(e, sibling, moveDown);
									if(moveRight.y > moveDown.y){
										res = moveDown;
									}else {
										if(moveRight.y == moveDown.y){
											if(moveRight.x > moveDown.x){
												res = moveDown;
											}else{
												res = moveRight;
											}
										}else {
											res = moveRight;
										}
									}
								}
								else {
									//does not fit to the right, move to next most left place
									res = moveDown;
									res = nextLeftFloatPositionLoop(e, sibling, res);
								}
							}
							break;
						}

						case LayoutFloat::RIGHT: {

							//rule 3
							if(s_left < res.x + e->getWidth() && res.y < s_bottom){
								//does not fit next to right floating sibling, move to the bottom of sibling
								res.y = s_bottom;
								res.x = 0;
								res = nextLeftFloatPositionLoop(e, sibling, res);
							}

							break;
						}

					}

				}
			}
		}
	}
	return res;
}

void FloatingBoxLayout::setNextRightFloatPosition(ofPoint p, Element* e){

	p = nextRightFloatPositionLoop(e, e, p);
	e->setPosition(p);

}

ofPoint FloatingBoxLayout::nextRightFloatPositionLoop(Element* e, Element* s, const ofPoint& pos){

	ofPoint res = pos;

	for (Element* sibling: children()){

		if(sibling == s){
			return res;
		}

		if(sibling){

			if(!sibling->isHidden()){

				float e_width = e->getWidth();
				float start_x = _parent->getWidth()-e_width;
				float s_top = sibling->getShape().getTop();
				float s_bottom = sibling->getShape().getBottom();
				float s_right = sibling->getShape().getRight();
				float s_left = sibling->getShape().getLeft();

				//rule 5, rule 6
				if(res.y < s_top){
					res.y = s_top;
					res.x = start_x;
					res = nextRightFloatPositionLoop(e, sibling, res);
				}

				if(sibling->hasAttribute("float")){

					LayoutFloat lf = sibling->getAttribute<LayoutFloat>("float");

					switch(lf){

						case LayoutFloat::RIGHT: {

							//rule 2, rule 7, rule 3
							if((res.x+e_width > s_left) && (res.y < s_bottom)){

								//align element on the left side of the sibling

								ofPoint moveLeft(s_left-e_width, res.y);
								ofPoint moveDown(start_x, s_bottom);

								if(moveLeft.x >= 0){
									moveLeft = nextRightFloatPositionLoop(e, sibling, moveLeft);
									moveDown = nextRightFloatPositionLoop(e, sibling, moveDown);
									if(moveLeft.y > moveDown.y){
										res = moveDown;
									}else {
										if(moveLeft.y == moveDown.y){
											if(moveLeft.x < moveDown.x){
												res = moveDown;
											}else{
												res = moveLeft;
											}
										}else {
											res = moveLeft;
										}
									}
								}
								else {
									//does not fit to the left, move to next most right place
									res = moveDown;
									res = nextRightFloatPositionLoop(e, sibling, res);
								}
							}
							break;
						}

						case LayoutFloat::NONE:
						default:
						case LayoutFloat::LEFT: {

							//rule 3
							if(s_right > res.x && res.y < s_bottom){
								//does not fit next to left floating sibling, move to the bottom of sibling
								res.y = s_bottom;
								res.x = start_x;
								res = nextRightFloatPositionLoop(e, sibling, res);
							}

							break;
						}

					}

				}

			}

		}
	}

	return res;

}

}} // namespace ofx::DOM
