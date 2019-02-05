
	//  if you need any help with this code, please send an email to
	// detlef.diesing@uni-due.de



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

# define NUMBER_OF_COLUMNS 40  // surface size in j (horizontal) direction, the default value taken in

# define NUMBER_OF_ROWS 40     // surface size in i (vertical) direction , the default value taken in

# define TIME_DELAY_SUBSTRATE_CIRCLES_SHOW_UP 0.01


# define UPPER_LIMIT_FOR_SUBSTRATE_DISPLAY  149 // this is to limit the size of the eps output files

	// if UPPER_LIMIT_FOR_SUBSTRATE_DISPLAY is larger than NUMBER_OF_COLUMNS, then all atoms  of two layers of the substrate (C , B) and the adatom layer are displayed.
	// the reader should try it least one time to set UPPER_LIMIT_FOR_SUBSTRATE_DISPLAY > NUMBER_OF_COLUMNS to have the optical impression of the crystal layers of the 111 surface


	// Chemical constants

# define R 8.3144
# define PREEXPFACTOR_DESORPTION 0
# define PREEXPFACTOR_DIFFUSION 5e12
# define ELECTRON_CHARGE 1.6019e-19
# define K_BOLTZMANN 1.3807e-23
# define H_BAR 1.05457e-34
# define H_PLANCK 6.62606957e-34
# define Pi 3.1415927

#define STROKE_WIDTH 0.1

#define NUMBER_OF_DIFFUSION_STEPS 850

#define SCREEN_TIME_FOR_ONE_STEP 0.02


#define DIFFUSION_ACTIVATION_ENERGY 0.10

#define TEMPERATURE 150


	// In line 52 oder 55  there is the path where your data will be



//# define PROGRAMM_DIRECTORY "/Users/diesing/programming/output_support/nano_growth_ripen/random_walk/"


# define PROGRAMM_DIRECTORY "/Users/diesing/programming/output_support/random_walk_local/"



	//  # define PROGRAMM_DIRECTORY ""






struct MCS_STRUCT{

	int ***number_of_visits_tensor;
	int **actual_position_of_adsorbate;
	int mc_step;
	int central_i;
	int central_j;
	int i_coordinate_adsorbate; // i coordinate of the adatom
	int j_coordinate_adsorbate; // j coordinate of the adatom



	double random_number; //random number values chosen by procedure mc_step
	double mc_time;
	double time_increment;
	double temperature;
	double single_adsorbate_diffusion_time;


} mcs;


int j_periodic(int j)
{
	int output=0;

	if (j>=1 && j<=NUMBER_OF_COLUMNS) {
		output=j;

	}

	if (j==0) {
		output=NUMBER_OF_COLUMNS;
	}

	if (j==-1) {
		output=NUMBER_OF_COLUMNS-1;
	}

	if (j==-2) {
		output=NUMBER_OF_COLUMNS-2;
	}



	if (j==NUMBER_OF_COLUMNS+1) {
		output=1;
	}

	if (j==NUMBER_OF_COLUMNS+2) {
		output=2;
	}

	if (j==NUMBER_OF_COLUMNS+3) {
		output=3;
	}




	return output;

}


int i_periodic(int i)
{
	int output=0;
	if (i>=1 && i<=NUMBER_OF_ROWS) {
		output=i;

	}

	if (i==0) {
		output=NUMBER_OF_ROWS;
	}

	if (i==-1) {
		output=NUMBER_OF_ROWS-1;
	}

	if (i==-2) {
		output=NUMBER_OF_ROWS-2;
	}





	if (i==NUMBER_OF_ROWS+1) {
		output=1;
	}


	if (i==NUMBER_OF_ROWS+2) {
		output=2;
	}

	if (i==NUMBER_OF_ROWS+3) {
		output=3;
	}

	return output;

}






int random_1_6(void)
{
	int output_value;

	output_value=1+(rand() % 6);

	return output_value;
}




void check_the_number_of_visits_matrix(int num_diff_steps,struct MCS_STRUCT *ptr_to_mcs)
{

	ptr_to_mcs=&mcs;

	int i=0,j=0;

	for (i=1; i<=NUMBER_OF_ROWS; i++) {

		fprintf(stdout, "Row %03i ",i);

		for (j=1; j<=NUMBER_OF_COLUMNS; j++) {

			fprintf(stdout, " %i ",mcs.number_of_visits_tensor[num_diff_steps][i][j]);

		}

		fprintf(stdout, "\n");


	}


	fprintf(stdout, "\n");



}


void data_output_animated_svg(struct MCS_STRUCT *ptr_to_mcs){

	ptr_to_mcs=&mcs;

	FILE *fhd1;

	int i=0,j=0,number_of_visits_local=0,i_local=0,j_local=0,mc_step_local=0;

	char filename_output[200]="";

	char filename_concepts_import[200]="";

	double screen_time_for_one_step=SCREEN_TIME_FOR_ONE_STEP;

	double time_estimated_for_whole_animation=screen_time_for_one_step*NUMBER_OF_DIFFUSION_STEPS;

	double screen_time=0.0;


	strcat(filename_output,PROGRAMM_DIRECTORY);

	strcat(filename_output, "output_animated.svg");

	strcat(filename_concepts_import, PROGRAMM_DIRECTORY);

	strcat(filename_concepts_import,"pfeilgruppe2.txt");



		//	double page_margin=0.1;

		//	double stroke_width_page_margin=0.3;

	double final_page_width=82.0*3.0;

	double tentative_page_height=final_page_width/(sqrt(2.));


	double relative_sphere_radius=1.0;

	double sphere_radius=tentative_page_height/(2.*relative_sphere_radius+NUMBER_OF_ROWS*sqrt(3)*relative_sphere_radius);


	double measured_page_height=sphere_radius*(2.-sqrt(3)/3.+sqrt(3)*NUMBER_OF_ROWS);

	double measured_page_width=4*sphere_radius+(NUMBER_OF_COLUMNS+NUMBER_OF_ROWS/2.-3./2.)*sphere_radius*2;


	double correctur_factor=measured_page_width/measured_page_height;

	double page_width=final_page_width;

	double page_height=page_width/correctur_factor;


		// the following two x _coordinates (smallest and largest) are used to make a rectangular output instead of a paralleggramm output

	int text_font_size_pts=4;

	double text_font_size_mm=text_font_size_pts*25.4/72.0;

	double text_x_offset=8.0;

	double legend_y_offset=10.0;

	double text_line_spacing=1.5*text_font_size_pts;

	int red_value=0,green_value=0,blue_value=0;

	double smallest_x_coordinate=0.0;

	double largest_x_coordinate=0.0;

	double x_translate=0.0;


		// recalculation of the sphere radius


	sphere_radius=page_height/(2.*relative_sphere_radius+NUMBER_OF_ROWS*sqrt(3)*relative_sphere_radius);


		// Layer C is shifted towards layer B //

		// The shift in x-direction is called delta_x , its value is sphere_radius
		// The shift in x-direction is called delta_y , its value is minus sphere_radius times sqrt(3)/3, this is also tan(30deg)
		// the later comes from the geometrical calculation that the layer C atom is just in the middle of
		// an equilateral triangle which is spanned by three atoms of the layer B (the layer below C)

	double  delta_x=sphere_radius;

	double  delta_y=sphere_radius*tan(M_PI/6);

	double x_coordinate=0.0,y_coordinate=0.0;

	double x_coordinate_actual_position_legend=0.0,y_coordinate_actual_position_legend=0.0;







		// define the parallelogramm which replaces layer c for greater matrices

		// seen from the top left corner of the page

	/*	double
	 top_left_corner_parallelogram_x=2*sphere_radius ,
	 top_left_corner_parallelogram_y=-1.*sphere_radius*(1.+sqrt(3)/3.),


	 bottom_left_corner_parallelogram_x=sphere_radius+sphere_radius*NUMBER_OF_ROWS,

	 bottom_left_corner_parallelogram_y=(.1547005390*sphere_radius-1.732050808*sphere_radius*NUMBER_OF_ROWS),

	 top_right_corner_parallelogram_x=2*sphere_radius*NUMBER_OF_COLUMNS,

	 top_right_corner_parallelogram_y= top_left_corner_parallelogram_y, // ist ungeaendert

	 bottom_right_corner_parallelogram_x=-1.*sphere_radius+2.*sphere_radius*NUMBER_OF_COLUMNS+sphere_radius*NUMBER_OF_ROWS,

	 bottom_right_corner_parallelogram_y=(.1547005390*sphere_radius-1.732050808*sphere_radius*NUMBER_OF_ROWS);

	 */













		// some helpers for svg

		// The following lines are taken from w3c.org

	char svg_header_line_one[91]="<?xml version=\"1.0\" standalone=\"no\"?>";

	char svg_header_line_two[]="<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">";

		// line three contains the page size and is defined later

	char svg_header_line_three[200]="";

	char svg_header_line_four[]="xmlns=\"http://www.w3.org/2000/svg\"";

	char svg_header_line_five[]="xmlns:xlink=\"http://www.w3.org/1999/xlink\">";

		// you find a help for this here: https://stackoverflow.com/questions/29884930/svg-namespace-prefix-xlink-for-href-on-textpath-is-not-defined

		//  define the header line three

		//	sprintf(svg_header_line_three,"<svg  viewBox=\"%g %g %g %g\" width=\"%gmm\" height=\"%gmm\" version=\"1.1\" ",(X_OFFSET_PERCENT/100.*imag_par.page_width),(Y_OFFSET_PERCENT/100.*imag_par.page_height) ,(imag_par.page_width/MAGNIFICATION_IN_VIEW_BOX),(imag_par.page_height/MAGNIFICATION_IN_VIEW_BOX),imag_par.page_width,imag_par.page_height);


	sprintf(svg_header_line_three,"<svg  viewBox=\"0 0 %i %i\" width=\"%gmm\" height=\"%gmm\" version=\"1.1\" ",(int)(ceil(page_width)),(int)(ceil(page_height)),page_width,page_height);


	fhd1=fopen(filename_output,"w+");

	fprintf(stdout, "%s\n",filename_output);



	if (fhd1==NULL) {

		printf("Error in opening the file with name\n -%s- ",filename_output);

		printf("\n\nplease check line number 47 in the programm code \n may be add or remove a / ? \n\n ");


	}

	else {


		printf("\n\n A  svg file  named \n -->%s<-- \nwas successfully written \n ",filename_output);


	}



	fhd1=fopen(filename_output,"w+");

	fprintf(fhd1,"%s\n",svg_header_line_one);

	fprintf(fhd1,"%s\n",svg_header_line_two);

	fprintf(fhd1,"%s\n",svg_header_line_three);

	fprintf(fhd1,"%s\n",svg_header_line_four);

	fprintf(fhd1,"%s\n",svg_header_line_five);



		// this double for loop fixes the smallest and largest x-coordinates to enable the rectangular cut of the parallelogramm and enable the positioning of the legend

	for (i=1; i<=NUMBER_OF_ROWS; i++) {

		for (j=1; j<=NUMBER_OF_COLUMNS; j++){

			x_coordinate=(double)(sphere_radius+((j-1)+(i-1)/2.0)*(sphere_radius*2));
				// the upper line is the shift of each row with index i >1


			y_coordinate=(double)((sphere_radius+1*((i-1)*sphere_radius*sqrt(3))));


			if (i==NUMBER_OF_ROWS && j==1) {



				smallest_x_coordinate=x_coordinate;


			}


			if (i==1 && j==NUMBER_OF_COLUMNS) {

				largest_x_coordinate=x_coordinate;


			}





		}

	}



		// now the translation of the x can be fixed

	x_translate=-smallest_x_coordinate+sphere_radius;








		// now we can fix positions in the legend

	x_coordinate_actual_position_legend=largest_x_coordinate+text_x_offset+sphere_radius;


	y_coordinate_actual_position_legend=legend_y_offset+11.0*text_line_spacing;



		//  The layers B, C and the adsorbate layer are shifted vs each other

		//  by sphere_radius in x and by sphere_radius*tan(30 deg) in y

		//  This is the reason for the above definition of

		//	double  delta_x=sphere_radius;

		//  double  delta_y=sphere_radius*tan(M_PI/6);








		// write the substrate layer B

	fprintf(fhd1, "<defs> <g id=\"substrate layer B\">\n");

	fprintf(fhd1, "<style type=\"text/css\"> <![CDATA[  \n");

		// die 64 sind (1-0.75)*255 , 0.75 war im eps output codiert

	fprintf(fhd1,"circle {stroke:rgb(64,64,64); stroke-width:%g; fill:rgb(64,64,64);stroke-opacity:1.0;fill-opacity:1.0 } \n",STROKE_WIDTH);

	fprintf(fhd1, "]]>\n</style>\n  ");

	for (i=1; i<=NUMBER_OF_ROWS; i++) {

		for (j=1; j<=NUMBER_OF_COLUMNS; j++){

			x_coordinate=(double)(sphere_radius+((j-1)+(i-1)/2.0)*(sphere_radius*2));
				// the upper line is the shift of each row with index i >1


			y_coordinate=(double)((sphere_radius+1*((i-1)*sphere_radius*sqrt(3))));

			if (x_coordinate>=smallest_x_coordinate && x_coordinate<=largest_x_coordinate) {


					//    fprintf(stdout,"<circle cx=\"%g\" cy=\"%g\" r=\"%g\" />\n",x_coordinate,y_coordinate,sphere_radius);



				fprintf(fhd1,"<circle cx=\"%g\" cy=\"%g\" r=\"%g\" />\n",x_coordinate,y_coordinate,sphere_radius);


			}

		}

	}

	fprintf(fhd1,"</g>\n</defs>\n");

	fprintf(fhd1, "<use xlink:href=\"#substrate layer B\" transform=\"translate(%g,%g)\" />\n\n",-smallest_x_coordinate+sphere_radius,0.0);





		// write the substrate layer C


	fprintf(fhd1, "<defs> <g id=\"substrate layer C\">\n");

		// fprintf(fhd1, "<style type=\"text/css\"> <![CDATA[  \n");

		// die 128 sind 0.5 *255 , so war das codiert im eps output




	for (i=1; i<=NUMBER_OF_ROWS; i++) {

		for (j=1; j<=NUMBER_OF_COLUMNS; j++){


			x_coordinate=(double)(delta_x+sphere_radius+((j-1)+(i-1)/2.0)*(sphere_radius*2));

			y_coordinate=(double) (delta_y+1*(sphere_radius+(i-1)*sphere_radius*sqrt(3)));




			if (x_coordinate>=smallest_x_coordinate && x_coordinate<=largest_x_coordinate) {


				fprintf(fhd1,"<circle cx=\"%g\" cy=\"%g\" r=\"%g\" style=\" stroke:rgb(128,128,128); stroke-width:%g; fill:rgb(128,128,128);stroke-opacity:1.0;fill-opacity:1.0; \"  />\n",x_coordinate,y_coordinate,sphere_radius,STROKE_WIDTH);


					//     fprintf(stdout,"<circle cx=\"%g\" cy=\"%g\" r=\"%g\" />\n",x_coordinate,y_coordinate,sphere_radius);






			}

		}

	}


	fprintf(fhd1,"</g>\n</defs>\n");

	fprintf(fhd1, "<use xlink:href=\"#substrate layer C\" transform=\"translate(%g,%g)\" />\n\n",-smallest_x_coordinate+sphere_radius,0.0);








		// Now write the legend //


	fprintf(fhd1, "\n\n<defs> <g id=\"write the legend\" style=\"font-size:%ipt;font-family:Helvetica\" >\n",text_font_size_pts);


	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset,legend_y_offset);

	fprintf(fhd1, " Number of stays \n");

	fprintf(fhd1, "</text>\n\n");




	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+sphere_radius,legend_y_offset+text_line_spacing,sphere_radius,0,0,0,0,0,0);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+6*sphere_radius,legend_y_offset+text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " 1 \n");

	fprintf(fhd1, "</text>\n\n");


	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+sphere_radius,legend_y_offset+2.0*text_line_spacing,sphere_radius,145,42,42,145,42,42);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+6*sphere_radius,legend_y_offset+2.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " 2 \n");

	fprintf(fhd1, "</text>\n\n");



	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+sphere_radius,legend_y_offset+3.0*text_line_spacing,sphere_radius,194,34,11,194,34,11);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+6*sphere_radius,legend_y_offset+3.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " 3 \n");

	fprintf(fhd1, "</text>\n\n");



	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+sphere_radius,legend_y_offset+4.0*text_line_spacing,sphere_radius,2444,34,11,2444,34,11);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+6*sphere_radius,legend_y_offset+4.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " 4 \n");

	fprintf(fhd1, "</text>\n\n");




	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+sphere_radius,legend_y_offset+5.0*text_line_spacing,sphere_radius,255,44,84,255,44,84);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+6*sphere_radius,legend_y_offset+5.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " 5 \n");

	fprintf(fhd1, "</text>\n\n");



	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+sphere_radius,legend_y_offset+6.0*text_line_spacing,sphere_radius,255,114,0,255,114,0);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+6*sphere_radius,legend_y_offset+6.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " 6 \n");

	fprintf(fhd1, "</text>\n\n");


	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+sphere_radius,legend_y_offset+7.0*text_line_spacing,sphere_radius,255,165,0,255,165,0);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+6*sphere_radius,legend_y_offset+7.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, "7");

	fprintf(fhd1, "</text>\n\n");




	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+sphere_radius,legend_y_offset+8.0*text_line_spacing,sphere_radius,218,165,32,218,165,32);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+6*sphere_radius,legend_y_offset+8.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " 8 \n");

	fprintf(fhd1, "</text>\n\n");



	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+sphere_radius,legend_y_offset+9.0*text_line_spacing,sphere_radius,255,215,0,255,215,0);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n",largest_x_coordinate+text_x_offset+4*sphere_radius,legend_y_offset+9.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " > 8 \n");

	fprintf(fhd1, "</text>\n\n");

	fprintf(fhd1, "<!-- Write the symbol for the start position  in the legend-->\n");



	fprintf(fhd1,"<circle cx=\"%g\" cy=\"%g\" r=\"%g\" style=\" stroke:rgb(0,0,0); stroke-width:%g; fill:rgb(255,255,255);stroke-opacity:1.0;fill-opacity:1.0; \"  />\n", largest_x_coordinate+text_x_offset+sphere_radius,legend_y_offset+10.0*text_line_spacing,sphere_radius,STROKE_WIDTH*2);

	fprintf(fhd1, "<line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\"  style=\"stroke:black; stroke-width:%g; stroke-opacity:1.0\" /> \n",largest_x_coordinate+text_x_offset,legend_y_offset+10.0*text_line_spacing,largest_x_coordinate+text_x_offset+2.0*sphere_radius,legend_y_offset+10.0*text_line_spacing,STROKE_WIDTH*2);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+4*sphere_radius,legend_y_offset+10.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " start position \n");

	fprintf(fhd1, "</text>\n\n\n");



	fprintf(fhd1, "<!-- Write the symbol for the actual position-->\n");



	fprintf(fhd1,"<circle cx=\"%g\" cy=\"%g\" r=\"%g\" style=\" stroke:rgb(0,0,0); stroke-width:%g; fill:rgb(255,255,255);stroke-opacity:1.0;fill-opacity:1.0; \"  />\n", largest_x_coordinate+text_x_offset+sphere_radius,legend_y_offset+11.0*text_line_spacing,sphere_radius,STROKE_WIDTH*2);

		// the next two lines are the x

	fprintf(fhd1, "<line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\"  style=\"stroke:black; stroke-width:%g; stroke-opacity:1.0\" /> \n",largest_x_coordinate+text_x_offset+sphere_radius-sphere_radius/sqrt(2) ,legend_y_offset+11.0*text_line_spacing-sphere_radius/sqrt(2),largest_x_coordinate+text_x_offset+sphere_radius+sphere_radius/sqrt(2),legend_y_offset+11.0*text_line_spacing+sphere_radius/sqrt(2),STROKE_WIDTH*2);

	fprintf(fhd1, "<line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\"  style=\"stroke:black; stroke-width:%g; stroke-opacity:1.0\" /> \n",largest_x_coordinate+text_x_offset+sphere_radius-sphere_radius/sqrt(2) ,legend_y_offset+11.0*text_line_spacing+sphere_radius/sqrt(2),largest_x_coordinate+text_x_offset+sphere_radius+sphere_radius/sqrt(2),legend_y_offset+11.0*text_line_spacing-sphere_radius/sqrt(2),STROKE_WIDTH*2);



	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+4*sphere_radius,legend_y_offset+11.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, "actual position \n");

	fprintf(fhd1, "</text>\n\n\n\n");

	fprintf(fhd1,"</g>\n</defs>\n");

	fprintf(fhd1, "<use xlink:href=\"#write the legend\" transform=\"translate(%g,%g)\" />\n\n\n",-smallest_x_coordinate+sphere_radius,0.0);







		// now write the trace of the adsorbate




	for (mc_step_local=0; mc_step_local<=NUMBER_OF_DIFFUSION_STEPS; mc_step_local++) {


		screen_time=screen_time_for_one_step*mc_step_local;


		if (mc_step_local==0) {


				//	fprintf(fhd1, "\n<g id=\"Trace_Adsorbate_Step%03i\" >\n",mc_step_local);


			i_local=mcs.actual_position_of_adsorbate[mc_step_local][1];

			j_local=mcs.actual_position_of_adsorbate[mc_step_local][2];

			x_coordinate=(double) (2*delta_x+sphere_radius+((j_local-1)+(i_local-1)/2.0)*(sphere_radius*2)) + x_translate;
				// the upper line is the shift of each row with index i >1

			y_coordinate=(double) (2*delta_y+1*(sphere_radius+1*((i_local-1)*sphere_radius*2*pow(3,0.5)/2.0)));

			number_of_visits_local=mcs.number_of_visits_tensor[mc_step_local][i_local][j_local]; // this is not really needed here, since we write the start position.


			fprintf(fhd1, "\n<g style=\"opacity:0.0;\" >\n");


				// jetzt muss er nur die Mitte Schreiben




			fprintf(fhd1,"<circle cx=\"%g\" cy=\"%g\" r=\"%g\" style=\" stroke:rgb(0,0,0); stroke-width:%g; fill:rgb(255,255,255);stroke-opacity:1.0;fill:1.0;\" />\n\n",x_coordinate,y_coordinate,sphere_radius,STROKE_WIDTH*2);



			fprintf(fhd1, "<line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\"  style=\"stroke:black; stroke-width:%g; stroke-opacity:1.0; \" /> \n",x_coordinate-sphere_radius,y_coordinate,x_coordinate+sphere_radius,y_coordinate,STROKE_WIDTH*2);



			fprintf(fhd1,"<!-- the i j coordinates of the line above are [%i,%i] -->\n",i_local,j_local);


			/*

			 fprintf(fhd1, "\n <g  style=\"font-size:%ipt;font-family:Helvetica;visibility:hidden;\" >\n",text_font_size_pts);

			 fprintf(fhd1,"<text x=\"%g\" y=\"%g\" style=\"visibility:hidden\" >\n",-smallest_x_coordinate+sphere_radius+largest_x_coordinate+text_x_offset+1*sphere_radius,legend_y_offset+13*text_line_spacing+text_font_size_mm);


			 fprintf(fhd1, "Diffusionsschritt Nr: %03i \n",mc_step_local);

			 fprintf(fhd1, "<set attributeName=\"visibility\" attributeType=\"CSS\" to=\"visible\" begin=\"1.0s\" end=\"2.0s\" fill=\"remove\"/>\n");

			 fprintf(fhd1, "</text>\n");

			 fprintf(fhd1,"\n</g>\n");

			 */

			fprintf(fhd1, "\n<animate id=\"intro\"  attributeName=\"opacity\" attributeType=\"XML\" from=\"0.0\" to=\"0.99\" begin=\"%gs\" dur=\"%gs\" fill=\"freeze\" />\n",0.0,2.0);

				// the line above pops up the start symbol





			fprintf(fhd1, "\n</g>\n");





		}


		if (mc_step_local==1) {

				// at first write the symbol for the actual position
				// this has to be done always

			screen_time=mc_step_local*screen_time_for_one_step;

			i_local=mcs.actual_position_of_adsorbate[mc_step_local][1];

			j_local=mcs.actual_position_of_adsorbate[mc_step_local][2];

			x_coordinate=(double) (2*delta_x+sphere_radius+((j_local-1)+(i_local-1)/2.0)*(sphere_radius*2)) + x_translate;
				// the upper line is the shift of each row with index i >1

			y_coordinate=(double) (2*delta_y+1*(sphere_radius+1*((i_local-1)*sphere_radius*2*pow(3,0.5)/2.0)));


			fprintf(fhd1, "\n<g  style=\"visibility:hidden;\" > \n");



			fprintf(fhd1,"<circle cx=\"%g\" cy=\"%g\" r=\"%g\" style=\" stroke:rgb(0,0,0); stroke-width:%g; fill:rgb(255,255,255);stroke-opacity:1.0;fill:1.0; \"  >\n",x_coordinate,y_coordinate,sphere_radius,STROKE_WIDTH*2);

			fprintf(fhd1,"</circle>\n");



			fprintf(fhd1, "<line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\"  style=\"stroke:black; stroke-width:%g; stroke-opacity:1.0\" /> \n",x_coordinate-sphere_radius/sqrt(2),y_coordinate-sphere_radius/sqrt(2),x_coordinate+sphere_radius/sqrt(2),y_coordinate+sphere_radius/sqrt(2),STROKE_WIDTH*2);


			fprintf(fhd1, "<line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\"  style=\"stroke:black; stroke-width:%g; stroke-opacity:1.0\" /> \n",x_coordinate-sphere_radius/sqrt(2),y_coordinate+sphere_radius/sqrt(2),x_coordinate+sphere_radius/sqrt(2),y_coordinate-sphere_radius/sqrt(2),STROKE_WIDTH*2);

				//fprintf(fhd1, "\n<animate   attributeName=\"opacity\" attributeType=\"XML\" from=\"0.0\" to=\"0.99\" begin=\"%gs\" end=\"%gs\" fill=\"freeze\" />\n",screen_time_for_one_step*mc_step_local,time_estimated_for_whole_animation);


			/*
			 fprintf(fhd1, "\n <g  style=\"font-size:%ipt;font-family:Helvetica;visibility:hidden;\" >\n",text_font_size_pts);

			 fprintf(fhd1,"<text x=\"%g\" y=\"%g\" style=\"visibility:hidden\" >\n",-smallest_x_coordinate+sphere_radius+largest_x_coordinate+text_x_offset+1*sphere_radius,legend_y_offset+13*text_line_spacing+text_font_size_mm);


			 fprintf(fhd1, "Diffusionsschritt Nr: %03i \n",mc_step_local);

			 fprintf(fhd1, "<set attributeName=\"visibility\" attributeType=\"CSS\" to=\"visible\" begin=\"%gs\" end=\"%gs\" fill=\"remove\"/>\n",screen_time+2.0,2*screen_time+2.0);

			 fprintf(fhd1, "</text>\n");

			 fprintf(fhd1,"\n</g>\n");

			 */

			fprintf(fhd1,"<set attributeName=\"visibility\" attributeType=\"CSS\" to=\"visible\" begin=\"%gs\" end=\"%gs\" fill=\"freeze\"  />  ",screen_time+2.0,time_estimated_for_whole_animation);


			fprintf(fhd1,"</g>\n\n");




		}


		if(mc_step_local>1){




				// now the adsorbate position of the monte carlo step before has to be shown in its real color, in the moment there is still the black x in white circle because it was the actual position before. This should be done only, if the start position is not overwritten.

			i_local=mcs.actual_position_of_adsorbate[mc_step_local-1][1];

			j_local=mcs.actual_position_of_adsorbate[mc_step_local-1][2];





			x_coordinate=(double) (2*delta_x+sphere_radius+((j_local-1)+(i_local-1)/2.0)*(sphere_radius*2)) + x_translate;
				// the upper line is the shift of each row with index i >1

			y_coordinate=(double) (2*delta_y+1*(sphere_radius+1*((i_local-1)*sphere_radius*2*pow(3,0.5)/2.0)));


			screen_time=screen_time_for_one_step*mc_step_local;

			number_of_visits_local=mcs.number_of_visits_tensor[mc_step_local-1][i_local][j_local];

			if (number_of_visits_local==1) {
				red_value=0;
				green_value=0;
				blue_value=0;

			}

			if (number_of_visits_local==2) {
				red_value=165;
				green_value=42;
				blue_value=42;

			}

			if (number_of_visits_local==3) {
				red_value=184;
				green_value=34;
				blue_value=11;

			}


			if (number_of_visits_local==4) {
				red_value=255;
				green_value=64;
				blue_value=0;

			}

			if (number_of_visits_local==5) {
				red_value=255;
				green_value=0;
				blue_value=0;

			}


			if (number_of_visits_local==5) {
				red_value=255;
				green_value=0;
				blue_value=64;

			}

			if (number_of_visits_local==6) {
				red_value=255;
				green_value=64;
				blue_value=0;

			}

			if (number_of_visits_local==7) {
				red_value=255;
				green_value=128;
				blue_value=0;

			}

			if (number_of_visits_local==8) {
				red_value=255;
				green_value=192;
				blue_value=0;

			}


			if (number_of_visits_local>8) {
				red_value=255;
				green_value=255;
				blue_value=0;

			}



			fprintf(fhd1, "\n<g  style=\"visibility:hidden;\" > \n");


			fprintf(fhd1,"<circle cx=\"%g\" cy=\"%g\" r=\"%g\" style=\" stroke:rgb(%i,%i,%i); stroke-width:%g; fill:rgb(%i,%i,%i);stroke-opacity:1.0;fill-opacity:1.0; \"  />\n",x_coordinate,y_coordinate,sphere_radius,red_value,green_value,blue_value,STROKE_WIDTH,red_value,green_value,blue_value);


			fprintf(fhd1,"<!-- the i j coordinates of the position where I have been before are [%i,%i] -->\n>",i_local,j_local);





				// now write the x on the actual position



			i_local=mcs.actual_position_of_adsorbate[mc_step_local][1];

			j_local=mcs.actual_position_of_adsorbate[mc_step_local][2];


			x_coordinate=(double) (2*delta_x+sphere_radius+((j_local-1)+(i_local-1)/2.0)*(sphere_radius*2)) + x_translate;
				// the upper line is the shift of each row with index i >1

			y_coordinate=(double) (2*delta_y+1*(sphere_radius+1*((i_local-1)*sphere_radius*2*pow(3,0.5)/2.0)));


			screen_time=screen_time_for_one_step*mc_step_local;

			fprintf(fhd1,"<circle cx=\"%g\" cy=\"%g\" r=\"%g\" style=\" stroke:rgb(0,0,0); stroke-width:%g; fill:rgb(255,255,255);stroke-opacity:1.0;fill:1.0; \"  >\n",x_coordinate,y_coordinate,sphere_radius,STROKE_WIDTH*2);

			fprintf(fhd1,"</circle>\n");



			fprintf(fhd1, "<line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\"  style=\"stroke:black; stroke-width:%g; stroke-opacity:1.0\" /> \n",x_coordinate-sphere_radius/sqrt(2),y_coordinate-sphere_radius/sqrt(2),x_coordinate+sphere_radius/sqrt(2),y_coordinate+sphere_radius/sqrt(2),STROKE_WIDTH*2);


			fprintf(fhd1, "<line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\"  style=\"stroke:black; stroke-width:%g; stroke-opacity:1.0\" /> \n",x_coordinate-sphere_radius/sqrt(2),y_coordinate+sphere_radius/sqrt(2),x_coordinate+sphere_radius/sqrt(2),y_coordinate-sphere_radius/sqrt(2),STROKE_WIDTH*2);



				// now rewrite the Startposition if it was overwritten by the step before





			x_coordinate=(double) (2*delta_x+sphere_radius+((mcs.central_j-1)+(mcs.central_i-1)/2.0)*(sphere_radius*2)) + x_translate;
				// the upper line is the shift of each row with index i >1

			y_coordinate=(double) (2*delta_y+1*(sphere_radius+1*((mcs.central_i-1)*sphere_radius*2*pow(3,0.5)/2.0)));

			fprintf(fhd1,"<circle cx=\"%g\" cy=\"%g\" r=\"%g\" style=\" stroke:rgb(0,0,0); stroke-width:%g; fill:rgb(255,255,255);stroke-opacity:1.0;fill:1.0;\" />\n\n",x_coordinate,y_coordinate,sphere_radius,STROKE_WIDTH*2);



			fprintf(fhd1, "<line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\"  style=\"stroke:black; stroke-width:%g; stroke-opacity:1.0; \" /> \n",x_coordinate-sphere_radius,y_coordinate,x_coordinate+sphere_radius,y_coordinate,STROKE_WIDTH*2);





			/*	fprintf(fhd1, "\n <g  style=\"font-size:%ipt;font-family:Helvetica;visibility:hidden;\" >\n",text_font_size_pts);

			 fprintf(fhd1,"<text x=\"%g\" y=\"%g\" style=\"visibility:hidden\" >\n",-smallest_x_coordinate+sphere_radius+largest_x_coordinate+text_x_offset+1*sphere_radius,legend_y_offset+13*text_line_spacing+text_font_size_mm);


			 fprintf(fhd1, "Diffusionsschritt Nr: %03i \n",mc_step_local);

			 fprintf(fhd1, "<set attributeName=\"visibility\" attributeType=\"CSS\" to=\"visible\" begin=\"%gs\" end=\"%gs\" fill=\"remove\" />\n",screen_time+2.0,5*screen_time+2.0);

			 fprintf(fhd1, "</text>\n");

			 fprintf(fhd1,"\n</g>\n");



			 fprintf(fhd1, "<rect x=\"%g\" y=\"%g\"  width=\"60\" height=\"30\" style=\"stroke:white;fill:white; visibility:hidden\" />\n",-smallest_x_coordinate+sphere_radius+largest_x_coordinate+text_x_offset+1*sphere_radius,legend_y_offset+12*text_line_spacing+text_font_size_mm);
			 */

			fprintf(fhd1,"<set attributeName=\"visibility\" attributeType=\"CSS\" to=\"visible\" begin=\"%gs\" end=\"%gs\" fill=\"freeze\"  />  ",screen_time+2.0,time_estimated_for_whole_animation);






			fprintf(fhd1,"</g>\n\n");











		}



	}



	fprintf(fhd1, "\n</svg>\n\n");

	fclose(fhd1);


}



void data_output_animated_html(struct MCS_STRUCT *ptr_to_mcs){

	ptr_to_mcs=&mcs;

	FILE *fhd1;

	int i=0,j=0,number_of_visits_local=0,i_local=0,j_local=0,mc_step_local=0;

	char filename_output[200]="";

	char filename_concepts_import[200]="";

	double screen_time_for_one_step=SCREEN_TIME_FOR_ONE_STEP;

	double time_estimated_for_whole_animation=screen_time_for_one_step*NUMBER_OF_DIFFUSION_STEPS;

	double screen_time=0.0;


	double x_start_of_the_word_position=21.0;

	strcat(filename_output,PROGRAMM_DIRECTORY);

	strcat(filename_output, "output_animated_new.html");

	strcat(filename_concepts_import, PROGRAMM_DIRECTORY);

	strcat(filename_concepts_import,"pfeilgruppe2.txt");



		//	double page_margin=0.1;

		//	double stroke_width_page_margin=0.3;

	double final_page_width=82.0*3.0;

	double tentative_page_height=final_page_width/(sqrt(2.));


	double relative_sphere_radius=1.0;

	double sphere_radius=tentative_page_height/(2.*relative_sphere_radius+NUMBER_OF_ROWS*sqrt(3)*relative_sphere_radius);


	double measured_page_height=sphere_radius*(2.-sqrt(3)/3.+sqrt(3)*NUMBER_OF_ROWS);

	double measured_page_width=4*sphere_radius+(NUMBER_OF_COLUMNS+NUMBER_OF_ROWS/2.-3./2.)*sphere_radius*2;


	double correctur_factor=measured_page_width/measured_page_height;

	double page_width=final_page_width;

	double page_height=page_width/correctur_factor;


		// the following two x _coordinates (smallest and largest) are used to make a rectangular output instead of a paralleggramm output

	int text_font_size_pts=4;

	double text_font_size_mm=text_font_size_pts*25.4/72.0;

	double text_x_offset=43.0;

	double legend_y_offset=5.0;

	double text_line_spacing=1.5*text_font_size_pts;

	int red_value=0,green_value=0,blue_value=0;

	double smallest_x_coordinate=0.0;

	double largest_x_coordinate=0.0;

	double x_translate=0.0;


		// recalculation of the sphere radius


	sphere_radius=page_height/(2.*relative_sphere_radius+NUMBER_OF_ROWS*sqrt(3)*relative_sphere_radius);


		// Layer C is shifted towards layer B //

		// The shift in x-direction is called delta_x , its value is sphere_radius
		// The shift in x-direction is called delta_y , its value is minus sphere_radius times sqrt(3)/3, this is also tan(30deg)
		// the later comes from the geometrical calculation that the layer C atom is just in the middle of
		// an equilateral triangle which is spanned by three atoms of the layer B (the layer below C)

	double  delta_x=sphere_radius;

	double  delta_y=sphere_radius*tan(M_PI/6);

	double x_coordinate=0.0,y_coordinate=0.0;

	double **four_edge_spheres_B_layer_coordinates;


//	double x_coordinate_actual_position_legend=0.0,y_coordinate_actual_position_legend=0.0;



// At first we define the parallelogramm of the B Layer, for this purpose we define the four points

	four_edge_spheres_B_layer_coordinates=(double **) calloc(5, sizeof(double));

	for (i=0; i<=4; i++) {
		four_edge_spheres_B_layer_coordinates[i]=(double *) calloc(3, sizeof(double));
	}



		// So four_edge_spheres_B_layer_coordinates[1][1] should be the x coordinate of the first point (top left)
		// four_edge_spheres_B_layer_coordinates[1][2] the y coordinate of this point
		// four_edge_spheres_B_layer_coordinates[2][1,2] the x,y coordinates of the bottom left
		// [3][1,2]  is the bottom right
		// [4][1,2]  is the top right










		// define the parallelogramm which replaces layer c for greater matrices

		// seen from the top left corner of the page

	/*	double
	 top_left_corner_parallelogram_x=2*sphere_radius ,
	 top_left_corner_parallelogram_y=1.*sphere_radius*(1.+sqrt(3)/3.),


	 bottom_left_corner_parallelogram_x=sphere_radius+sphere_radius*NUMBER_OF_ROWS,

	 bottom_left_corner_parallelogram_y=(.1547005390*sphere_radius+1.732050808*sphere_radius*NUMBER_OF_ROWS),

	 top_right_corner_parallelogram_x=2*sphere_radius*NUMBER_OF_COLUMNS,

	 top_right_corner_parallelogram_y= top_left_corner_parallelogram_y, // ist ungeaendert

	 bottom_right_corner_parallelogram_x=1.*sphere_radius+2.*sphere_radius*NUMBER_OF_COLUMNS+sphere_radius*NUMBER_OF_ROWS,

	 bottom_right_corner_parallelogram_y=(.1547005390*sphere_radius+1.732050808*sphere_radius*NUMBER_OF_ROWS);

*/

		// this double for loop determines the smallest and largest x-coordinates to enable the rectangular cut of the parallelogramm , it enable the positioning of the legend and it determines the x y coordinates of the 4 edge atoms which determines the parallelogram for the B layer

	for (i=1; i<=NUMBER_OF_ROWS; i++) {

		for (j=1; j<=NUMBER_OF_COLUMNS; j++){

			x_coordinate=(double)(sphere_radius+((j-1)+(i-1)/2.0)*(sphere_radius*2));
				// the upper line is the shift of each row with index i >1


			y_coordinate=(double)((sphere_radius+1*((i-1)*sphere_radius*sqrt(3))));



			if (i==1 && j ==1) {

				four_edge_spheres_B_layer_coordinates[1][1]=x_coordinate;
				four_edge_spheres_B_layer_coordinates[1][2]=y_coordinate;
			}

			if (i==NUMBER_OF_ROWS && j ==1) {

				four_edge_spheres_B_layer_coordinates[2][1]=x_coordinate;
				four_edge_spheres_B_layer_coordinates[2][2]=y_coordinate;
			}

			if (i==NUMBER_OF_ROWS && j ==NUMBER_OF_COLUMNS) {

				four_edge_spheres_B_layer_coordinates[3][1]=x_coordinate;
				four_edge_spheres_B_layer_coordinates[3][2]=y_coordinate;
			}


			if (i==1 && j ==NUMBER_OF_COLUMNS) {

				four_edge_spheres_B_layer_coordinates[4][1]=x_coordinate;
				four_edge_spheres_B_layer_coordinates[4][2]=y_coordinate;
			}









			if (i==NUMBER_OF_ROWS && j==1) {



				smallest_x_coordinate=x_coordinate;


			}


			if (i==1 && j==NUMBER_OF_COLUMNS) {

				largest_x_coordinate=x_coordinate;


			}





		}

	}

		// now the translation of the x can be fixed

	x_translate=smallest_x_coordinate+sphere_radius;









		// some helpers for svg and html

		// The following lines are taken from w3c.org

	char html_header[175]="<!DOCTYPE html>\n<html lang=\"en\">\n\n<head>\n<meta charset=\"utf-8\" />\n\n<title>\nAnimated random walk jep\n</title>\n\n</head>\n\n\n\n\n\n<body>\n";
    
    char html_link_to_css[200]="<link rel=\"stylesheet\" href=\"css/style.css\">\n";
    

	char html_closer[16]="</body>\n</html>";



		// line three contains the page size and is defined later

	char svg_header_1[200]="";

	char svg_header_2[200]="";


	sprintf(svg_header_1,"<svg  viewBox=\"0 0 %i %i\" width=\"%gmm\" height=\"%gmm\"  style=\"border:solid 0.05mm\" >\n",(int)(ceil(page_width)),(int)(ceil(page_height)),page_width,page_height);

	sprintf(svg_header_2,"<svg  viewBox=\"2 2 %i %i\" width=\"%gmm\" height=\"%gmm\"  style=\"border:solid 0.05mm \" >\n",(int)(ceil(page_width)),(int)(ceil(page_height)),page_width,page_height);



//sprintf(svg_header,"<svg  viewBox=\"0 0 %i %i\" width=\"%gmm\" height=\"%gmm\"\n>",(int)(ceil(page_width)),(int)(ceil(page_height)),page_width,page_height);




	char svg_closer[]="</svg>\n";


	fhd1=fopen(filename_output,"w+");

	fprintf(stdout, "%s\n",filename_output);



	if (fhd1==NULL) {

		printf("Error in opening the file with name\n -%s- ",filename_output);

		printf("\n\nplease check line number 47 in the programm code \n may be add or remove a / ? \n\n ");


	}

	else {


		printf("\n\n A  svg file  named \n -->%s<-- \nwas successfully written \n ",filename_output);


	}



	fhd1=fopen(filename_output,"w+");

	fprintf(fhd1,"%s\n",html_header);

    fprintf(fhd1, "%s \n",html_link_to_css);
    


	// Start writing the SVG

	fprintf(fhd1,"%s\n",svg_header_1);


		// prints the parallelogramm

	fprintf(fhd1, "<path d=\"M%g %g, L %g %g, L%g %g, L%g %g, Z  \" style=\"stroke:white; fill:none; stroke-width:0.05mm\";/>\n\n\n",
			four_edge_spheres_B_layer_coordinates[1][1],four_edge_spheres_B_layer_coordinates[1][2],
			four_edge_spheres_B_layer_coordinates[2][1],four_edge_spheres_B_layer_coordinates[2][2],
			four_edge_spheres_B_layer_coordinates[3][1],four_edge_spheres_B_layer_coordinates[3][2],
			four_edge_spheres_B_layer_coordinates[4][1],four_edge_spheres_B_layer_coordinates[4][2]
			);






		// write the substrate layer B


    // define the style for the substrate layer B
    
  //  fprintf(fhd1, "<style>\n");
    
    // die 64 sind (1-0.75)*255 , 0.75 war im eps output codiert
    
 //   fprintf(fhd1,"circle {stroke:rgb(64,64,64);\n stroke-width:%g;\n fill:rgb(64,64,64);\nopacity:1.0;\n",STROKE_WIDTH);
    
 //   fprintf(fhd1, "transition: fill .3s ease-in; ");
    
  //  fprintf(fhd1, "animate: attributeName=\"opacity\" attributeType=\"XML\" begin=\"0s\" dur=\"1s\" fill=\"freeze\" from=\"0\" to=\"1\" ; \n");
    
//    fprintf(fhd1," }\n " );
    
    
    
 //   fprintf(fhd1, "</style>\n\n");








	 // The inner html of this group is later changes by Javascript



	fprintf(fhd1,"<!-- The line below is the substrate layer B which is later changed by the Javascript -->\n\n");

	fprintf(fhd1, "<g id=\"substrate layer B\"></g>\n\n");



	fprintf(fhd1,"<!-- The line below is the substrate layer C which is later changed by the Javascript -->\n\n");

	fprintf(fhd1, "<g id=\"substrate layer C\"></g>\n\n");







		// Now write the legend //


	fprintf(fhd1, " \n<g id=\"write the legend\" style=\"font-size:%ipt;font-family:Helvetica\" >\n\n",text_font_size_pts);


	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset,legend_y_offset);

    fprintf(fhd1, "Number of stays: \n");

	fprintf(fhd1, "</text>\n\n");




	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+2*text_font_size_mm,legend_y_offset+text_line_spacing,text_font_size_mm,0,0,0,0,0,0);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+6*text_font_size_mm,legend_y_offset+text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " 1 \n");

	fprintf(fhd1, "</text>\n\n");


	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+2*text_font_size_mm,legend_y_offset+2.0*text_line_spacing,text_font_size_mm,145,42,42,145,42,42);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+6*text_font_size_mm,legend_y_offset+2.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " 2 \n");

	fprintf(fhd1, "</text>\n\n");



	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+2*text_font_size_mm,legend_y_offset+3.0*text_line_spacing,text_font_size_mm,194,34,11,194,34,11);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+6*text_font_size_mm,legend_y_offset+3.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " 3 \n");

	fprintf(fhd1, "</text>\n\n");



	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+2*text_font_size_mm,legend_y_offset+4.0*text_line_spacing,text_font_size_mm,2444,34,11,2444,34,11);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+6*text_font_size_mm,legend_y_offset+4.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " 4 \n");

	fprintf(fhd1, "</text>\n\n");




	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+2*text_font_size_mm,legend_y_offset+5.0*text_line_spacing,text_font_size_mm,255,44,84,255,44,84);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+6*text_font_size_mm,legend_y_offset+5.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " 5 \n");

	fprintf(fhd1, "</text>\n\n");



	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+2*text_font_size_mm,legend_y_offset+6.0*text_line_spacing,text_font_size_mm,255,114,0,255,114,0);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+6*text_font_size_mm,legend_y_offset+6.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " 6 \n");

	fprintf(fhd1, "</text>\n\n");


	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+2*text_font_size_mm,legend_y_offset+7.0*text_line_spacing,text_font_size_mm,255,165,0,255,165,0);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+6*text_font_size_mm,legend_y_offset+7.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, "7");

	fprintf(fhd1, "</text>\n\n");




	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+2*text_font_size_mm,legend_y_offset+8.0*text_line_spacing,text_font_size_mm,218,165,32,218,165,32);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+6*text_font_size_mm,legend_y_offset+8.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " 8 \n");

	fprintf(fhd1, "</text>\n\n");



	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+2*text_font_size_mm,legend_y_offset+9.0*text_line_spacing,text_font_size_mm,255,215,0,255,215,0);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n",largest_x_coordinate+text_x_offset+6.0*text_font_size_mm,legend_y_offset+9.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " > 8 \n");

	fprintf(fhd1, "</text>\n\n");



	fprintf(fhd1,"</g>\n");

	//fprintf(fhd1, "<use xlink:href=\"#write the legend\" transform=\"translate(%g,%g)\" />\n\n\n",0.0,0.0);






// Now write the symbols for the actual and the start position on the lower left side of the sheet.



	fprintf(fhd1, "\n\n <g id=\"write the left hand legend\" style=\"font-size:%ipt;font-family:Helvetica\" >\n\n",text_font_size_pts);

	fprintf(fhd1, "<!-- Write the symbol for the start position in the left hand legend  -->\n\n");



	fprintf(fhd1,"<circle cx=\"%g\" cy=\"%g\" r=\"%g\" style=\" stroke:rgb(0,0,0); stroke-width:%g; fill:rgb(255,255,255);stroke-opacity:1.0;fill-opacity:1.0; \"  />\n", text_font_size_mm,page_height-7.0*text_line_spacing,text_font_size_mm,STROKE_WIDTH*2);

	fprintf(fhd1, "<line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\"  style=\"stroke:black; stroke-width:%g; stroke-opacity:1.0\" /> \n",0.0,page_height-7.0*text_line_spacing,2.0*text_font_size_mm,page_height-7.0*text_line_spacing,STROKE_WIDTH*2);



	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",4*text_font_size_mm,page_height-7.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, "start\n");

	fprintf(fhd1, "</text>\n");

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",x_start_of_the_word_position,page_height-7.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, "position\n");

	fprintf(fhd1, "</text>\n\n");






	fprintf(fhd1, "<!-- Write the symbol for the actual position in the left hand legend -->\n\n");



	fprintf(fhd1,"<circle cx=\"%g\" cy=\"%g\" r=\"%g\" style=\" stroke:rgb(0,0,0); stroke-width:%g; fill:rgb(255,255,255);stroke-opacity:1.0;fill-opacity:1.0; \"  />\n", text_font_size_mm,page_height-6.0*text_line_spacing,text_font_size_mm,STROKE_WIDTH*2);

		// the next two lines are the x

	fprintf(fhd1, "<line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\"  style=\"stroke:black; stroke-width:%g; stroke-opacity:1.0\" /> \n",text_font_size_mm-text_font_size_mm/sqrt(2) ,page_height-6.0*text_line_spacing-text_font_size_mm/sqrt(2),text_font_size_mm+text_font_size_mm/sqrt(2),page_height-6.0*text_line_spacing+text_font_size_mm/sqrt(2),STROKE_WIDTH*2);

	fprintf(fhd1, "<line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\"  style=\"stroke:black; stroke-width:%g; stroke-opacity:1.0\" /> \n",text_font_size_mm-text_font_size_mm/sqrt(2) ,page_height-6.0*text_line_spacing+text_font_size_mm/sqrt(2),text_font_size_mm+text_font_size_mm/sqrt(2),page_height-6.0*text_line_spacing-text_font_size_mm/sqrt(2),STROKE_WIDTH*2);



	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",4*text_font_size_mm,page_height-6.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, "actual\n");

	fprintf(fhd1, "</text>\n\n\n\n");


	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",x_start_of_the_word_position,page_height-6.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, "position\n");

	fprintf(fhd1, "</text>\n\n");



	fprintf(fhd1,"<circle class=\"B_circle fade-in four \"  cx=\"%g\" cy=\"%g\" r=\"%g\" ; />\n\n", text_font_size_mm,page_height-5.0*text_line_spacing,text_font_size_mm);

	fprintf(fhd1, "<text class=\"fade-in four\" x=\"%g\"  y=\"%g\" >\n ",4*text_font_size_mm,page_height-5.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, "layer B\n");

	fprintf(fhd1, "</text>\n\n\n\n");



	fprintf(fhd1,"<circle class=\"C_circle fade-in ten \"  cx=\"%g\" cy=\"%g\" r=\"%g\" ; />\n\n", text_font_size_mm,page_height-4.0*text_line_spacing,text_font_size_mm);

	fprintf(fhd1, "<text class=\"fade-in ten\" x=\"%g\"  y=\"%g\" >\n ",4*text_font_size_mm,page_height-4.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, "layer C\n");

	fprintf(fhd1, "</text>\n\n\n\n");





	fprintf(fhd1, "</g> \n\n\n\n");

















	fprintf(fhd1,"%s\n",svg_closer);





	fprintf(fhd1,"<script>\n");




	fprintf(fhd1,"\t var the_B_circles_svg=\"\";\n\n");

	fprintf(fhd1,"\t\t function write_B_circles(NR,NC){\n");

	fprintf(fhd1,"\t\t var sphere_radius = %g\n",sphere_radius);

	fprintf(fhd1,"\t\t var x_coordinate=0.0;\n");

	fprintf(fhd1,"\t\t var y_coordinate=0.0;\n\n");

	fprintf(fhd1," \t\t for(var i=1; i<=NR; i++ ) {\n");

	fprintf(fhd1," \t\t for(var j=1; j<=NC; j++ ) {\n");

	fprintf(fhd1,"\t\t\t x_coordinate=sphere_radius+((j-1)+(i-1)/2.0)*(sphere_radius*2)\n");



	fprintf(fhd1,"\t\t\t y_coordinate=sphere_radius+1*((i-1)*sphere_radius*Math.sqrt(3))\n");

	//fprintf(fhd1,"\t\t console.log(x_coordinate)\n");

	fprintf(fhd1, "\t\t the_B_circles_svg += \"<circle class=\\\"B_circle fade-in four\\\" cx=\\\"\" \n");

	fprintf(fhd1," \t\t the_B_circles_svg += x_coordinate\n");

	fprintf(fhd1, "\t\t the_B_circles_svg += \"\\\"  cy=\\\"\" \n"  );

	fprintf(fhd1," \t\t the_B_circles_svg += y_coordinate\n");

	fprintf(fhd1, "\t\t the_B_circles_svg += \"\\\"  r=\\\"\" \n"  );

	fprintf(fhd1," \t\t the_B_circles_svg +=  sphere_radius\n");

	fprintf(fhd1, "\t\t the_B_circles_svg += \"\\\"  />\\n\"\n"  );




	//fprintf(fhd1," \t\t console.log(the_B_circles_svg)  \n\n ");



	fprintf(fhd1,"\t\t}\n");

	fprintf(fhd1,"\t}\n");

	fprintf(fhd1,"}\n\n\n");

	fprintf(fhd1,"write_B_circles(%i,%i)\n",NUMBER_OF_ROWS,NUMBER_OF_COLUMNS);

	//fprintf(fhd1,"console.log(the_B_circles_svg)\n\n\n");

	fprintf(fhd1,"document.getElementById(\"substrate layer B\").innerHTML = the_B_circles_svg; \n\n");





// The c-circles must be written

	fprintf(fhd1,"\t var the_C_circles_svg=\"\";\n\n");

	fprintf(fhd1,"\t\t function write_C_circles(NR,NC){\n");

	fprintf(fhd1,"\t\t var sphere_radius = %g\n",sphere_radius);

	fprintf(fhd1,"\t\t var x_coordinate=0.0;\n");

	fprintf(fhd1,"\t\t var y_coordinate=0.0;\n");

	fprintf(fhd1,"\t\t var delta_x=%g;\n\n",delta_x);

	fprintf(fhd1,"\t\t var delta_y=%g;\n\n",delta_y);


	fprintf(fhd1," \t\t for(var i=1; i<=NR; i++ ) {\n");

	fprintf(fhd1," \t\t for(var j=1; j<=NC; j++ ) {\n");

	fprintf(fhd1,"\t\t\t x_coordinate=delta_x+sphere_radius+((j-1)+(i-1)/2.0)*(sphere_radius*2)\n");



	fprintf(fhd1,"\t\t\t y_coordinate=delta_y+sphere_radius+1*((i-1)*sphere_radius*Math.sqrt(3))\n");

		//fprintf(fhd1,"\t\t console.log(x_coordinate)\n");

	fprintf(fhd1, "\t\t the_C_circles_svg += \"<circle class=\\\"C_circle fade-in ten\\\" cx=\\\"\" \n");

	fprintf(fhd1," \t\t the_C_circles_svg += x_coordinate\n");

	fprintf(fhd1, "\t\t the_C_circles_svg += \"\\\"  cy=\\\"\" \n"  );

	fprintf(fhd1," \t\t the_C_circles_svg += y_coordinate\n");

	fprintf(fhd1, "\t\t the_C_circles_svg += \"\\\"  r=\\\"\" \n"  );

	fprintf(fhd1," \t\t the_C_circles_svg +=  sphere_radius\n");

	fprintf(fhd1, "\t\t the_C_circles_svg += \"\\\"  />\\n\"\n"  );




		//fprintf(fhd1," \t\t console.log(the_C_circles_svg)  \n\n ");



	fprintf(fhd1,"\t\t}\n");

	fprintf(fhd1,"\t}\n");

	fprintf(fhd1,"}\n\n\n");

	fprintf(fhd1,"write_C_circles(%i,%i)\n",NUMBER_OF_ROWS,NUMBER_OF_COLUMNS);

		//fprintf(fhd1,"console.log(the_C_circles_svg)\n\n\n");

	fprintf(fhd1,"document.getElementById(\"substrate layer C\").innerHTML = the_C_circles_svg; \n\n");











	fprintf(fhd1,"</script>\n\n\n\n\n\n");









	fprintf(fhd1,"%s\n",html_closer);






		// Now write the legend //

/*
	fprintf(fhd1, "\n\n<defs> <g id=\"write the legend\" style=\"font-size:%ipt;font-family:Helvetica\" >\n",text_font_size_pts);


	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset,legend_y_offset);

	fprintf(fhd1, " Number of stays \n");

	fprintf(fhd1, "</text>\n\n");




	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+sphere_radius,legend_y_offset+text_line_spacing,sphere_radius,0,0,0,0,0,0);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+6*sphere_radius,legend_y_offset+text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " 1 \n");

	fprintf(fhd1, "</text>\n\n");


	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+sphere_radius,legend_y_offset+2.0*text_line_spacing,sphere_radius,145,42,42,145,42,42);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+6*sphere_radius,legend_y_offset+2.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " 2 \n");

	fprintf(fhd1, "</text>\n\n");



	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+sphere_radius,legend_y_offset+3.0*text_line_spacing,sphere_radius,194,34,11,194,34,11);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+6*sphere_radius,legend_y_offset+3.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " 3 \n");

	fprintf(fhd1, "</text>\n\n");



	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+sphere_radius,legend_y_offset+4.0*text_line_spacing,sphere_radius,2444,34,11,2444,34,11);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+6*sphere_radius,legend_y_offset+4.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " 4 \n");

	fprintf(fhd1, "</text>\n\n");




	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+sphere_radius,legend_y_offset+5.0*text_line_spacing,sphere_radius,255,44,84,255,44,84);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+6*sphere_radius,legend_y_offset+5.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " 5 \n");

	fprintf(fhd1, "</text>\n\n");



	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+sphere_radius,legend_y_offset+6.0*text_line_spacing,sphere_radius,255,114,0,255,114,0);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+6*sphere_radius,legend_y_offset+6.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " 6 \n");

	fprintf(fhd1, "</text>\n\n");


	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+sphere_radius,legend_y_offset+7.0*text_line_spacing,sphere_radius,255,165,0,255,165,0);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+6*sphere_radius,legend_y_offset+7.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, "7");

	fprintf(fhd1, "</text>\n\n");




	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+sphere_radius,legend_y_offset+8.0*text_line_spacing,sphere_radius,218,165,32,218,165,32);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+6*sphere_radius,legend_y_offset+8.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " 8 \n");

	fprintf(fhd1, "</text>\n\n");



	fprintf(fhd1, "<circle cx=\"%g\" cy=\"%g\" r=\"%g\"  style=\"stroke:rgb(%i,%i,%i);fill:rgb(%i,%i,%i); \"  />\n",largest_x_coordinate+text_x_offset+sphere_radius,legend_y_offset+9.0*text_line_spacing,sphere_radius,255,215,0,255,215,0);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n",largest_x_coordinate+text_x_offset+4*sphere_radius,legend_y_offset+9.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " > 8 \n");

	fprintf(fhd1, "</text>\n\n");

	fprintf(fhd1, "<!-- Write the symbol for the start position  in the legend-->\n");



	fprintf(fhd1,"<circle cx=\"%g\" cy=\"%g\" r=\"%g\" style=\" stroke:rgb(0,0,0); stroke-width:%g; fill:rgb(255,255,255);stroke-opacity:1.0;fill-opacity:1.0; \"  />\n", largest_x_coordinate+text_x_offset+sphere_radius,legend_y_offset+10.0*text_line_spacing,sphere_radius,STROKE_WIDTH*2);

	fprintf(fhd1, "<line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\"  style=\"stroke:black; stroke-width:%g; stroke-opacity:1.0\" /> \n",largest_x_coordinate+text_x_offset,legend_y_offset+10.0*text_line_spacing,largest_x_coordinate+text_x_offset+2.0*sphere_radius,legend_y_offset+10.0*text_line_spacing,STROKE_WIDTH*2);

	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+4*sphere_radius,legend_y_offset+10.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, " start position \n");

	fprintf(fhd1, "</text>\n\n\n");



	fprintf(fhd1, "<!-- Write the symbol for the actual position-->\n");



	fprintf(fhd1,"<circle cx=\"%g\" cy=\"%g\" r=\"%g\" style=\" stroke:rgb(0,0,0); stroke-width:%g; fill:rgb(255,255,255);stroke-opacity:1.0;fill-opacity:1.0; \"  />\n", largest_x_coordinate+text_x_offset+sphere_radius,legend_y_offset+11.0*text_line_spacing,sphere_radius,STROKE_WIDTH*2);

		// the next two lines are the x

	fprintf(fhd1, "<line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\"  style=\"stroke:black; stroke-width:%g; stroke-opacity:1.0\" /> \n",largest_x_coordinate+text_x_offset+sphere_radius-sphere_radius/sqrt(2) ,legend_y_offset+11.0*text_line_spacing-sphere_radius/sqrt(2),largest_x_coordinate+text_x_offset+sphere_radius+sphere_radius/sqrt(2),legend_y_offset+11.0*text_line_spacing+sphere_radius/sqrt(2),STROKE_WIDTH*2);

	fprintf(fhd1, "<line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\"  style=\"stroke:black; stroke-width:%g; stroke-opacity:1.0\" /> \n",largest_x_coordinate+text_x_offset+sphere_radius-sphere_radius/sqrt(2) ,legend_y_offset+11.0*text_line_spacing+sphere_radius/sqrt(2),largest_x_coordinate+text_x_offset+sphere_radius+sphere_radius/sqrt(2),legend_y_offset+11.0*text_line_spacing-sphere_radius/sqrt(2),STROKE_WIDTH*2);



	fprintf(fhd1, "<text x=\"%g\"  y=\"%g\" >\n ",largest_x_coordinate+text_x_offset+4*sphere_radius,legend_y_offset+11.0*text_line_spacing+text_font_size_mm);

	fprintf(fhd1, "actual position \n");

	fprintf(fhd1, "</text>\n\n\n\n");

	fprintf(fhd1,"</g>\n</defs>\n");

	fprintf(fhd1, "<use xlink:href=\"#write the legend\" transform=\"translate(%g,%g)\" />\n\n\n",-smallest_x_coordinate+sphere_radius,0.0);







		// now write the trace of the adsorbate

		// Dieses write the trace of the adsorbate wird in die Datei old_code_snippets.c in der Zeilen von 71 bis 423 verschoben am 22. Januar 2019






	fprintf(fhd1, "\n</svg>\n\n");




 */

	fclose(fhd1);

	for(i=0;i<=4;i++)
	{

		free(four_edge_spheres_B_layer_coordinates[i]);


	}

	free(four_edge_spheres_B_layer_coordinates);



}





void perform_diffusion(struct MCS_STRUCT *ptr_to_mcs)
{
	ptr_to_mcs=&mcs;


		// roll the dice ->

	int diffusion_direction=random_1_6();

	int i_initial=mcs.i_coordinate_adsorbate;
	int j_initial=mcs.j_coordinate_adsorbate;

	int i_final=0;
	int j_final=0;

		// this procedure is called for the first time with mcs.mc_step=1;


	int i=0,j=0;


		// the next double for loops transfer the number of visits matrix to the next layer

	for (i=1; i<=NUMBER_OF_ROWS; i++) {

		for (j=1; j<=NUMBER_OF_COLUMNS; j++) {

			mcs.number_of_visits_tensor[mcs.mc_step][i][j]=mcs.number_of_visits_tensor[mcs.mc_step-1][i][j];

		}

	}




	if (diffusion_direction==1) {

			// diffusion direction 1
			//  i - >  i and j - > j+1

		i_final=i_periodic(i_initial);
		j_final=j_periodic(j_initial+1);

		mcs.number_of_visits_tensor[mcs.mc_step][i_final][j_final]=mcs.number_of_visits_tensor[mcs.mc_step-1][i_final][j_final]+1;




	}


	if (diffusion_direction==2) {

			// diffusion direction 2
			//  i - > i-1 and j - > j+1


		i_final=i_periodic(i_initial-1);
		j_final=j_periodic(j_initial+1);


		mcs.number_of_visits_tensor[mcs.mc_step][i_final][j_final]=mcs.number_of_visits_tensor[mcs.mc_step-1][i_final][j_final]+1;



	}


	if (diffusion_direction==3) {

			// diffusion direction 3
			//  i - > i-1 and j - > j


		i_final=i_periodic(i_initial-1);
		j_final=j_periodic(j_initial);

		mcs.number_of_visits_tensor[mcs.mc_step][i_final][j_final]=mcs.number_of_visits_tensor[mcs.mc_step-1][i_final][j_final]+1;


	}


	if (diffusion_direction==4) {

			// diffusion direction 4
			//  i - > i and j - > j-1


		i_final=i_periodic(i_initial);
		j_final=j_periodic(j_initial-1);



		mcs.number_of_visits_tensor[mcs.mc_step][i_final][j_final]=mcs.number_of_visits_tensor[mcs.mc_step-1][i_final][j_final]+1;


	}


	if (diffusion_direction==5) {

			// diffusion direction 5
			//  i - > i+1 and j - > j-1


		i_final=i_periodic(i_initial+1);
		j_final=j_periodic(j_initial-1);


		mcs.number_of_visits_tensor[mcs.mc_step][i_final][j_final]=mcs.number_of_visits_tensor[mcs.mc_step-1][i_final][j_final]+1;


	}

	if (diffusion_direction==6) {

			// diffusion direction 6
			//  i - > i+1 and j - > j

		i_final=i_periodic(i_initial+1);
		j_final=j_periodic(j_initial);

		mcs.number_of_visits_tensor[mcs.mc_step][i_final][j_final]=mcs.number_of_visits_tensor[mcs.mc_step-1][i_final][j_final]+1;


	}




	mcs.i_coordinate_adsorbate=i_final;
	mcs.j_coordinate_adsorbate=j_final;

	mcs.actual_position_of_adsorbate[mcs.mc_step][1]=i_final;
	mcs.actual_position_of_adsorbate[mcs.mc_step][2]=j_final;


	mcs.mc_time+=mcs.single_adsorbate_diffusion_time;

		//printf("For step %02i  the [i,j] coordinates are [%i,%i] \n",mcs.mc_step,i_final,j_final);


}



int main(void)
{


	struct MCS_STRUCT *ptr_to_mcs;

	ptr_to_mcs=&mcs;


		// the line below  should be set active if you want to work with different random numbers every time you run the programm.

	srand((unsigned int) time(NULL));


		// if you want to work with always the same set of random numbers, place an // in front of srand(.....


		//  fprintf(stdout,"test %03.6g \n ",trunc(2.99));



	mcs.central_i=(int) (round(NUMBER_OF_ROWS/2));

	mcs.central_j=(int) (round(NUMBER_OF_COLUMNS/2));


	double single_adsorbate_diffusion_time=0.0;

	single_adsorbate_diffusion_time=1/(PREEXPFACTOR_DIFFUSION*exp(-ELECTRON_CHARGE*DIFFUSION_ACTIVATION_ENERGY/(K_BOLTZMANN*TEMPERATURE)));

	mcs.single_adsorbate_diffusion_time=single_adsorbate_diffusion_time;


	fprintf(stdout,"\n\nSingle adsorbate diffusion rate is %g \n\n",1/mcs.single_adsorbate_diffusion_time);

	fprintf(stdout,"\n\nSingle adsorbate diffusion time is %g \n\n",mcs.single_adsorbate_diffusion_time);

	int i=0,j=0,k=0;


	mcs.number_of_visits_tensor=(int ***)calloc((NUMBER_OF_DIFFUSION_STEPS+1),sizeof(int **));

	for (k=0; k<=NUMBER_OF_DIFFUSION_STEPS; k++) {

		mcs.number_of_visits_tensor[k]=(int **) calloc(NUMBER_OF_ROWS+1, sizeof(int *));

		for(i=0;i<=NUMBER_OF_ROWS;i++){

			mcs.number_of_visits_tensor[k][i]=(int *)calloc((NUMBER_OF_COLUMNS+1),sizeof(int));
		}

	}



	mcs.actual_position_of_adsorbate=(int **) calloc(NUMBER_OF_DIFFUSION_STEPS+1, sizeof(int *) );

	for (k=0; k<=NUMBER_OF_DIFFUSION_STEPS; k++) {

		mcs.actual_position_of_adsorbate[k]=(int *) calloc(3, sizeof(int));

			// it is 3 since 0 is ommitted ->  [mc_step][omitted,i_adsorbate,j_adsorbate]

	}


		//  Initialize the MC Simulation. With mc_step=0 one adatom is dropped on the surface

	mcs.mc_step=0;


	printf("For step %i the [i,j] coordinates [%i,%i] \n",mcs.mc_step,mcs.central_i,mcs.central_j);

	mcs.actual_position_of_adsorbate[0][1]=mcs.central_i;
	mcs.actual_position_of_adsorbate[0][2]=mcs.central_j;






		// set all other values to zero

	for (i=1; i<=NUMBER_OF_ROWS; i++) {

		for (j=1; j<=NUMBER_OF_COLUMNS; j++) {

			mcs.number_of_visits_tensor[1][i][j]=0;

			if (i==mcs.central_i && j == mcs.central_j) {
				mcs.number_of_visits_tensor[1][i][j]=1;
			}


		}
	}






	mcs.i_coordinate_adsorbate=mcs.central_i;

	mcs.j_coordinate_adsorbate=mcs.central_j;




		// with mc_step=1 the atom starts to run away from the central point of the matrix

	for (mcs.mc_step=1; mcs.mc_step<=NUMBER_OF_DIFFUSION_STEPS ; mcs.mc_step++) {

		perform_diffusion(&mcs);

			//	data_output_svg(&mcs);

			//	fprintf(stdout, "Monte Carlo Time is %3.3g sec at MC step %02i \n",mcs.mc_time,mcs.mc_step);


	}


	//data_output_animated_svg(&mcs);


	data_output_animated_html(&mcs);


		//check_the_number_of_visits_matrix(15,&mcs);




	return 0;




}






/*
 fprintf(fhd1, "<!-- Import von Concepts hier -->\n");


 fhd2=fopen(filename_concepts_import,"r");


 if (fhd2==NULL) {
 fprintf(stdout, "ein Fehler beim Import von Concepts ");
 }

 else {

 while(fgets(buffer,sizeof(buffer),fhd2)){
 fprintf(fhd1, "%s",buffer);
 }

 }


 fprintf(fhd1, "\n<!-- Ende des Imports von Concepts hier -->\n");

 fclose(fhd2);

 */









