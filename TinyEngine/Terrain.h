#pragma once
namespace TEngine {
	class MyTif;
	class Object;
	class Terrain {

	public:
		static Object* Create(MyTif& tif);
	};
}