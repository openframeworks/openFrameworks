//
//class ofProperty{
//	public:
//		
//		ofProperty(){
//			bMinMaxSet = false;
//		}
//		
//		ofProperty(T value){
//			val		= value;
//			bMinMaxSet = false;
//		}
//
//		ofProperty(T value, T min, T max){
//			val		= value;
//			valMin	= min;
//			valMax	= max;
//			bMinMaxSet = true;
//		}
//		
//		void setValue(T value){
//			val = value;
//		}
//		
//		void setValue(T value, T min, T max){
//			val		= value;
//			valMin	= min;
//			valMax	= max;
//			bMinMaxSet = true;
//		}
//		
//		void setMinMax(T min, T max){
//			valMin		= min;
//			valMax		= max;
//			bMinMaxSet	= true;
//		}
//		
//		T & getValue(){
//			return val;
//		}
//
//		T & getMinValue(){
//			return valMin;
//		}
//
//		T & getMaxValue(){
//			return valMax;
//		}
//		
//		T & operator = (T const & value){
//			val = value;
//			return val;
//		}
//		
//		ofProperty & operator = (ofProperty const & prop){
//			val		= prop.value;
//			valMin	= prop.valMin;
//			valMax	= prop.valMax;
//			return *this;
//		}		
//		
//		T val;
//		T valMax;
//		T valMin;
//		bool bMinMaxSet;
//};
//
//
