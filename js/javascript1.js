
var NUMBER_OF_ROWS = 200;

var MAX_NUMBER_OF_ROWS_ALL_CIRCLES = 5

var NUMBER_OF_COLUMNS = NUMBER_OF_ROWS;

var page_height = 150;

var page_width = 250;

var sphere_radius = 0.0;

var tentative_sphere_radius=page_height/( (NUMBER_OF_ROWS)*Math.sqrt(3)  );

var tentative_delta_x = tentative_sphere_radius;

var tentative_delta_y = tentative_sphere_radius*Math.tan(Math.PI/6);


	// the delta_x and delta_y mean the shift of the crystal layers in x and y direction






var four_edge_circles_B = [[0.0,0.0,0.0],[0.0,0.0,0.0],[0.0,0.0,0.0],[0.0,0.0,0.0],[0.0,0.0,0.0]];

	// four_edge_circles_B[1][1] means the x coordinaten of the the upper left point

	// four_edge_circles_B[1][2] means the 2 coordinaten of the the upper left point

	// four_edge_circles_B[2][1] means the 2 coordinaten of the the lower  left point and so on



var four_edge_circles_C = [[0.0,0.0,0.0],[0.0,0.0,0.0],[0.0,0.0,0.0],[0.0,0.0,0.0],[0.0,0.0,0.0]];






function determine_the_sphere_radius(number_of_rows,number_of_columns){


	var largest_extension_in_x_direction = tentative_delta_x + tentative_delta_x + tentative_sphere_radius+( ( (number_of_columns-1)+((number_of_rows-1)/2.0) ) *  (tentative_sphere_radius*2) ) + tentative_sphere_radius ;

		//  see comments for the largest extension in y direction

	var largest_extension_in_y_direction = tentative_delta_y + tentative_delta_y + tentative_sphere_radius +(  (number_of_rows-1) * tentative_sphere_radius * Math.sqrt(3)  ) + tentative_sphere_radius;

		// there is two times delta_y because we calculate for the adsorbate layer
		// one delta y would be enough to come from the B-layer to the C - layer
		// the next delta y is to from the C-layer to the adsorbate layer
		// the tentative_sphere_radius at the end is to come from from the center of the lowest row (highest i index) to the the edge of the lowest row sphere



		// console.log("control of the largest x extension");

		// console.log(tentative_sphere_radius);

		// console.log(largest_extension_in_x_direction);

		// console.log(largest_extension_in_y_direction);

	var correction_fator_x = largest_extension_in_x_direction / page_width;

	var correction_fator_y= largest_extension_in_y_direction / page_height;

	if(correction_fator_x > correction_fator_y){

		sphere_radius = tentative_sphere_radius / correction_fator_x;

	}


	if(correction_fator_y > correction_fator_x){

		sphere_radius = tentative_sphere_radius / correction_fator_y;

	}

	sphere_radius = Math.floor(sphere_radius*100)/100

		// console.log("the tentative sphere radius");

		// console.log(tentative_sphere_radius);



		// console.log("the real sphere radius");

		// console.log(sphere_radius);


}

determine_the_sphere_radius(NUMBER_OF_ROWS,NUMBER_OF_COLUMNS);



var delta_x = sphere_radius;

var delta_y = sphere_radius*Math.tan(Math.PI/6);







function determine_the_edge_points_B_parallelogram(page_height,page_width,number_of_rows)
{

	var number_of_columns = number_of_rows;





	delta_x = sphere_radius;

	delta_y = sphere_radius*Math.tan(Math.PI/6);






		// console.log(sphere_radius);

		// console.log("have a look \n");

	for (var i=1; i<=number_of_rows; i++) {

		for (var j=1; j<=number_of_columns; j++){

			x_coordinate=(sphere_radius+((j-1)+(i-1)/2.0)*(sphere_radius*2));
				// the upper line is the shift of each row with index i >1


			y_coordinate=((sphere_radius+1*((i-1)*sphere_radius*Math.sqrt(3))));




			if (i==1 && j ==1) {

				four_edge_circles_B[1][1] = Math.floor(x_coordinate*100)/100;
				four_edge_circles_B[1][2] = Math.floor(y_coordinate*100)/100;

					// console.log(four_edge_circles_B);

					// console.log(Math.floor(x_coordinate*100)/100);
					// console.log(Math.floor(y_coordinate*100(/100);

					// console.log(four_edge_circles_B[1][1]);

					// console.log(four_edge_circles_B[1][2]);









			}

			if (i==number_of_rows && j ==1) {

				four_edge_circles_B[2][1] = Math.floor(x_coordinate*100)/100;
				four_edge_circles_B[2][2] = Math.floor(y_coordinate*100)/100;

			}

			if (i==number_of_rows && j == number_of_columns) {

				four_edge_circles_B[3][1] = Math.floor(x_coordinate*100)/100;
				four_edge_circles_B[3][2] = Math.floor(y_coordinate*100)/100;

			}


			if (i==1 && j == number_of_columns) {

				four_edge_circles_B[4][1] = Math.floor(x_coordinate*100)/100;
				four_edge_circles_B[4][2] = Math.floor(y_coordinate*100)/100;

			}



		}

	}



}

determine_the_edge_points_B_parallelogram(page_height,page_width,NUMBER_OF_ROWS);


var the_B_circles_svg="";

var the_B_circles_parallelogram_svg = "";

function write_B_circles(number_of_rows,number_of_columns){


	if(number_of_rows < MAX_NUMBER_OF_ROWS_ALL_CIRCLES && number_of_columns < MAX_NUMBER_OF_ROWS_ALL_CIRCLES ) {
		var x_coordinate=0.0;
		var y_coordinate=0.0;

		if(number_of_rows <= MAX_NUMBER_OF_ROWS_ALL_CIRCLES)
		{
			for(var i=1; i<=number_of_rows; i++ ) {
				for(var j=1; j<=number_of_columns; j++ ) {
					x_coordinate=sphere_radius+((j-1)+(i-1)/2.0)*(sphere_radius*2)
					y_coordinate=sphere_radius+( (i-1) * sphere_radius*Math.sqrt(3))
					the_B_circles_svg += "<circle class=\"B_circle fade-inB B_circle\" cx=\""
					the_B_circles_svg += Math.floor(x_coordinate*100)/100
					the_B_circles_svg += "\"  cy=\""
					the_B_circles_svg += Math.floor(y_coordinate*100)/100
					the_B_circles_svg += "\"  r=\""
					the_B_circles_svg +=  sphere_radius
					the_B_circles_svg += "\"  />\n"
				}
			}
		}
	}



	else {

		for(var i=1; i<=number_of_rows; i++ ) {
			for(var j=1; j<=number_of_columns; j++ ) {

				if(i <=2 || j <= 2 || i >= number_of_rows -1  || j >= number_of_columns -1)
				{
					x_coordinate=sphere_radius+((j-1)+(i-1)/2.0)*(sphere_radius*2)
					y_coordinate=sphere_radius+( (i-1) * sphere_radius*Math.sqrt(3))
					the_B_circles_svg += "<circle class=\"B_circle fade-inB B_circle\" cx=\""
					the_B_circles_svg += Math.floor(x_coordinate*100)/100
					the_B_circles_svg += "\"  cy=\""
					the_B_circles_svg += Math.floor(y_coordinate*100)/100
					the_B_circles_svg += "\"  r=\""
					the_B_circles_svg +=  sphere_radius
					the_B_circles_svg += "\"  />\n"
				}
			}
		}

		the_B_circles_parallelogram_svg += "<path d=\"M "

		the_B_circles_parallelogram_svg += four_edge_circles_B[1][1] + " "

		the_B_circles_parallelogram_svg += four_edge_circles_B[1][2] + ", " + " L "

		the_B_circles_parallelogram_svg += four_edge_circles_B[2][1] + " "

		the_B_circles_parallelogram_svg += four_edge_circles_B[2][2] + ", " + " L "

		the_B_circles_parallelogram_svg += four_edge_circles_B[3][1] + " "

		the_B_circles_parallelogram_svg += four_edge_circles_B[3][2] + ", " + " L "

		the_B_circles_parallelogram_svg += four_edge_circles_B[4][1] + " "

		the_B_circles_parallelogram_svg += four_edge_circles_B[4][2] + ", " + " Z " + " \" "

		the_B_circles_parallelogram_svg +=  "class=\"B_line fade-inB B_line\"  /> "







	}


}


write_B_circles(NUMBER_OF_ROWS,NUMBER_OF_COLUMNS)

document.getElementById("substrate layer circles B").innerHTML = the_B_circles_svg;

document.getElementById("substrate layer parallelogram B").innerHTML = the_B_circles_parallelogram_svg;







function determine_the_edge_points_C_parallelogram(page_height,page_width,number_of_rows)
{

	var number_of_columns = number_of_rows;





	delta_x = sphere_radius;

	delta_y = sphere_radius*Math.tan(Math.PI/6);






		// console.log(sphere_radius);

		// console.log("have a look \n");

	for (var i=1; i<=number_of_rows; i++) {

		for (var j=1; j<=number_of_columns; j++){

			x_coordinate = delta_x + (sphere_radius+((j-1)+(i-1)/2.0)*(sphere_radius*2));
				// the upper line is the shift of each row with index i >1


			y_coordinate= delta_y + ((sphere_radius+1*((i-1)*sphere_radius*Math.sqrt(3))));




			if (i==1 && j ==1) {

				four_edge_circles_C[1][1] = Math.floor(x_coordinate*100)/100;
				four_edge_circles_C[1][2] = Math.floor(y_coordinate*100)/100;

					// console.log(four_edge_circles_B);

					// console.log(Math.floor(x_coordinate*100)/100);
					// console.log(Math.floor(y_coordinate*100(/100);

					// console.log(four_edge_circles_B[1][1]);

					// console.log(four_edge_circles_B[1][2]);









			}

			if (i==number_of_rows && j ==1) {

				four_edge_circles_C[2][1] = Math.floor(x_coordinate*100)/100;
				four_edge_circles_C[2][2] = Math.floor(y_coordinate*100)/100;

			}

			if (i==number_of_rows && j == number_of_columns) {

				four_edge_circles_C[3][1] = Math.floor(x_coordinate*100)/100;
				four_edge_circles_C[3][2] = Math.floor(y_coordinate*100)/100;

			}


			if (i==1 && j == number_of_columns) {

				four_edge_circles_C[4][1] = Math.floor(x_coordinate*100)/100;
				four_edge_circles_C[4][2] = Math.floor(y_coordinate*100)/100;

			}



		}

	}



}

determine_the_edge_points_C_parallelogram(page_height,page_width,NUMBER_OF_ROWS);

var the_C_circles_svg="";

var the_C_circles_parallelogram_svg = "";

function write_C_circles(number_of_rows,number_of_columns){


	if(number_of_rows < MAX_NUMBER_OF_ROWS_ALL_CIRCLES && number_of_columns < MAX_NUMBER_OF_ROWS_ALL_CIRCLES ) {
		var x_coordinate=0.0;
		var y_coordinate=0.0;

		if(number_of_rows <= MAX_NUMBER_OF_ROWS_ALL_CIRCLES)
		{
			for(var i=1; i<=number_of_rows; i++ ) {
				for(var j=1; j<=number_of_columns; j++ ) {
					x_coordinate = delta_x + sphere_radius+((j-1)+(i-1)/2.0)*(sphere_radius*2)
					y_coordinate = delta_y + sphere_radius+( (i-1) * sphere_radius*Math.sqrt(3))
					the_C_circles_svg += "<circle class=\"C_circle fade-inC C_circle\" cx=\""
					the_C_circles_svg += Math.floor(x_coordinate*100)/100
					the_C_circles_svg += "\"  cy=\""
					the_C_circles_svg += Math.floor(y_coordinate*100)/100
					the_C_circles_svg += "\"  r=\""
					the_C_circles_svg +=  sphere_radius
					the_C_circles_svg += "\"  />\n"
				}
			}
		}
	}



	else {

		for(var i=1; i<=number_of_rows; i++ ) {
			for(var j=1; j<=number_of_columns; j++ ) {

				if(i <=2 || j <= 2 || i >= number_of_rows -1  || j >= number_of_columns -1)
				{
					x_coordinate = delta_x + sphere_radius+((j-1)+(i-1)/2.0)*(sphere_radius*2)
					y_coordinate = delta_y + sphere_radius+( (i-1) * sphere_radius*Math.sqrt(3))
					the_C_circles_svg += "<circle class=\"C_circle fade-inC C_circle\" cx=\""
					the_C_circles_svg += Math.floor(x_coordinate*100)/100
					the_C_circles_svg += "\"  cy=\""
					the_C_circles_svg += Math.floor(y_coordinate*100)/100
					the_C_circles_svg += "\"  r=\""
					the_C_circles_svg +=  sphere_radius
					the_C_circles_svg += "\"  />\n"
				}
			}
		}

		the_C_circles_parallelogram_svg += "<path d=\"M "

		the_C_circles_parallelogram_svg += four_edge_circles_C[1][1] + " "

		the_C_circles_parallelogram_svg += four_edge_circles_C[1][2] + ", " + " L "

		the_C_circles_parallelogram_svg += four_edge_circles_C[2][1] + " "

		the_C_circles_parallelogram_svg += four_edge_circles_C[2][2] + ", " + " L "

		the_C_circles_parallelogram_svg += four_edge_circles_C[3][1] + " "

		the_C_circles_parallelogram_svg += four_edge_circles_C[3][2] + ", " + " L "

		the_C_circles_parallelogram_svg += four_edge_circles_C[4][1] + " "

		the_C_circles_parallelogram_svg += four_edge_circles_C[4][2] + ", " + " Z " + " \" "

		the_C_circles_parallelogram_svg +=  "class=\"C_line fade-inC C_line\"  /> "







	}


}


write_C_circles(NUMBER_OF_ROWS,NUMBER_OF_COLUMNS)

document.getElementById("substrate layer circles C").innerHTML = the_C_circles_svg;

document.getElementById("substrate layer parallelogram C").innerHTML = the_C_circles_parallelogram_svg;














