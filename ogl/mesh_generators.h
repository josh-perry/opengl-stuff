#pragma once

std::vector<float> generate_cube(float x, float y, float z)
{
	std::vector<float> p = {
		-x, -y, -z,
		-x, -y,  z,
		-x,  y,  z,
		 x,  y, -z,
		-x, -y, -z,
		-x,  y, -z,

		 x, -y,  z,
		-x, -y, -z,
		 x, -y, -z,
		 x,  y, -z,
		 x, -y, -z,
		-x, -y, -z,

		-x, -y, -z,
		-x,  y,  z,
		-x,  y, -z,
		 x, -y,  z,
		-x, -y,  z,
		-x, -y, -z,

		-x,  y,  z,
		-x, -y,  z,
		 x, -y,  z,
		 x,  y,  z,
		 x, -y, -z,
		 x,  y, -z,

		 x, -y, -z,
		 x,  y,  z,
		 x, -y,  z,
		 x,  y,  z,
		 x,  y, -z,
		-x,  y, -z,

		 x,  y,  z,
		-x,  y, -z,
		-x,  y,  z,
		 x,  y,  z,
		-x,  y,  z,
		 x, -y,  z
	};

	return p;
}

std::vector<float> generate_cube_ccw(float x, float y, float z)
{
	std::vector<float> p = {
	  -x,  y, -z,
	  -x, -y, -z,
	   x, -y, -z,
	   x, -y, -z,
	   x,  y, -z,
	  -x,  y, -z,

	  -x, -y,  z,
	  -x, -y, -z,
	  -x,  y, -z,
	  -x,  y, -z,
	  -x,  y,  z,
	  -x, -y,  z,

	   x, -y, -z,
	   x, -y,  z,
	   x,  y,  z,
	   x,  y,  z,
	   x,  y, -z,
	   x, -y, -z,

	  -x, -y,  z,
	  -x,  y,  z,
	   x,  y,  z,
	   x,  y,  z,
	   x, -y,  z,
	  -x, -y,  z,

	  -x,  y, -z,
	   x,  y, -z,
	   x,  y,  z,
	   x,  y,  z,
	  -x,  y,  z,
	  -x,  y, -z,

	  -x, -y, -z,
	  -x, -y,  z,
	   x, -y, -z,
	   x, -y, -z,
	  -x, -y,  z,
	   x, -y,  z
	};

	return p;
}
