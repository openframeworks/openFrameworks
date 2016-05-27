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

				if(elementFloating(element)){
					// TODO ignoring Orientation right now
					// because it is not clear if this is device orientation (my guess)
					// or horizontal vs. vertical alignment of elements (which the code implied)

					LayoutFloat layoutfloat = element->getAttribute<LayoutFloat>("float");

					switch(layoutfloat){

						case LayoutFloat::LEFT: {

							setNextLeftFloatPosition(ofPoint(0,0), element);


							break;
						}

						case LayoutFloat::RIGHT: {

							setNextRightFloatPosition(ofPoint(_parent->getWidth()-getWidth(element),0), element);

							break;
						}

						case LayoutFloat::NONE:
						default:{

							setPosition(element, ofPoint(currentX, currentY));

							break;
						}

					}

					totalWidth = std::max(totalWidth, getRight(element));
					currentY = std::max(currentY, getBottom(element));
					totalHeight = std::max(totalHeight, getBottom(element));


				}
			}
		}

//		if(!_parent->usesPercentalWidth()){
//			totalWidth = std::max(totalWidth, _parent->getShape().getWidth());
//			_parent->setWidth(totalWidth);
//		}
		_parent->setHeight(totalHeight);

		_isDoingLayout = false;
	}

}

bool FloatingBoxLayout::elementFloating(Element* e){
	if(!e->isHidden()){
		if(e->hasAttribute("position")){
			if(e->getAttribute<LayoutPosition>("position") == LayoutPosition::STATIC){
				if(e->hasAttribute("float")){
					return true;
				}
			}
		}
	}
	return false;
}

///\brief floating layout based on https://www.w3.org/TR/CSS2/visuren.html#float-rules
void FloatingBoxLayout::setNextLeftFloatPosition(ofPoint p, Element* e){

	p = nextLeftFloatPositionLoop(e, e, p);
	setPosition(e, p);

}

ofPoint FloatingBoxLayout::nextLeftFloatPositionLoop(Element* e, Element* s, const ofPoint& pos){

	ofPoint res = pos;

	for (Element* sibling: children()){

		if(sibling == s){
			return res;
		}

		if(sibling){

			if(elementFloating(sibling)){

				float s_top = getTop(sibling);
				float s_bottom = getBottom(sibling);
				float s_right = getRight(sibling);
				float s_left = getLeft(sibling);

				//rule 5, rule 6
				if(res.y < s_top){
					res.y = s_top;
					res.x = 0;
					res = nextLeftFloatPositionLoop(e, sibling, res);
				}

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

							if(s_right + getWidth(e) <= _parent->getWidth()){
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
						if(s_left < res.x + getWidth(e) && res.y < s_bottom){
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
	return res;
}

void FloatingBoxLayout::setNextRightFloatPosition(ofPoint p, Element* e){

	p = nextRightFloatPositionLoop(e, e, p);
	setPosition(e, p);

}

ofPoint FloatingBoxLayout::nextRightFloatPositionLoop(Element* e, Element* s, const ofPoint& pos){

	ofPoint res = pos;

	for (Element* sibling: children()){

		if(sibling == s){
			return res;
		}

		if(sibling){

			if(elementFloating(sibling)){

				float e_width = getWidth(e);
				float start_x = _parent->getWidth()-e_width;
				float s_top = getTop(sibling);
				float s_bottom = getBottom(sibling);
				float s_right = getRight(sibling);
				float s_left = getLeft(sibling);

				//rule 5, rule 6
				if(res.y < s_top){
					res.y = s_top;
					res.x = start_x;
					res = nextRightFloatPositionLoop(e, sibling, res);
				}

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

	return res;

}

float FloatingBoxLayout::getRight(Element* e){
	if(e->hasAttribute("margin-right")){
		return e->getShape().getRight() + e->getAttribute<float>("margin-right");
	}else {
		return e->getShape().getRight();
	}
}

float FloatingBoxLayout::getLeft(Element* e){
	if(e->hasAttribute("margin-left")){
		return e->getShape().getLeft() - e->getAttribute<float>("margin-left");
	}else {
		return e->getShape().getLeft();
	}
}

float FloatingBoxLayout::getTop(Element* e){
	if(e->hasAttribute("margin-top")){
		return e->getShape().getTop() - e->getAttribute<float>("margin-top");
	}else {
		return e->getShape().getTop();
	}
}

float FloatingBoxLayout::getBottom(Element* e){
	if(e->hasAttribute("margin-bottom")){
		return e->getShape().getBottom() + e->getAttribute<float>("margin-bottom");
	}else {
		return e->getShape().getBottom();
	}
}

float FloatingBoxLayout::getWidth(Element* e){
	float res = e->getWidth();
	if(e->hasAttribute("margin-left")){
		res += e->getAttribute<float>("margin-left");
	}
	if(e->hasAttribute("margin-right")){
		res += e->getAttribute<float>("margin-right");
	}
	return res;
}

float FloatingBoxLayout::getHeight(Element* e){
	float res = e->getHeight();
	if(e->hasAttribute("margin-top")){
		res += e->getAttribute<float>("margin-top");
	}
	if(e->hasAttribute("margin-bottom")){
		res += e->getAttribute<float>("margin-bottom");
	}
	return res;
}

void FloatingBoxLayout::setPosition(Element* e, ofPoint p){
	if(e->hasAttribute("margin-left")){
		p.x += e->getAttribute<float>("margin-left");
	}
	if(e->hasAttribute("margin-top")){
		p.y += e->getAttribute<float>("margin-top");
	}
	e->setPosition(p);
}

}} // namespace ofx::DOM
