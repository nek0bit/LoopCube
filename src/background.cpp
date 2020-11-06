#include "background.hpp"

Background::Background()
	: win_width{0}, show_cave_background{false}, bg_shine_src{}, bg_shine_dest{}, bg_cloud_offset_x{0}, bg_cloud_offset_y{0}, bg_hills_extend{131, 131, 131, 255}, bg_cave_block_x{0}, bg_cave_block_y{0}, dark{0, .25}, darkness{0} {
	// Set size for objects
	bg_shine_w = 10; //px
	bg_shine_h = 150;

	bg_cloud_w = 450;
	bg_cloud_h = 100;

	bg_hills_w = 450*2;
	bg_hills_h = 114*2;

	bg_cave_block_w = 60;
	bg_cave_block_h = 60;

	bg_light_w = 100;
	bg_light_h = 100;
}

Background::~Background() {}

void Background::update(Camera& camera, Time& time) {
	const int light_width = bg_light_w;
	const int light_height = bg_light_h;
	const int cave_background_offset = -7269; // How deep down until the cave background shows
	const int light_cam_left = (camera.get_width()-(light_width))/2;
	const int light_cam_top = 20;
	const int hills_offset = -100;
	const double time_over_max = static_cast<double>(time.time) / static_cast<double>(time.max_time);
	const int hor_circle = camera.get_width()*.35;
	const int vert_circle = camera.get_height()/4;
	const int darkness_amount = 180;
	const int day_color_r = 106, day_color_g = 164, day_color_b = 222;
	
	win_width = camera.get_width();
	win_height = camera.get_height();
	// Update bg_shine
	bg_shine_src.x = 0;
	bg_shine_src.y = 0;
	bg_shine_src.w = bg_shine_w;
	bg_shine_src.h = bg_shine_h;

	bg_shine_dest.x = 0;
	bg_shine_dest.y = 0;
	bg_shine_dest.w = camera.get_width();
	bg_shine_dest.h = camera.get_height();

	// Update bg_cloud
	bg_cloud_offset_x = camera.get_x()/10;
	bg_cloud_offset_y = camera.get_y()/30;

	// Update hills
	bg_hills_offset_x = camera.get_x()/15;
	bg_hills_offset_y = (camera.get_height()/2)+hills_offset;

	// Enable the cave background is camera is low enough
	show_cave_background = camera.get_y() < cave_background_offset;
	bg_cave_block_x = camera.get_x()/10;
	bg_cave_block_y = camera.get_y()/10;

	//********************************
	//  Handle time
	//********************************
	dark.update();

	// Transition to Morning
	// Else if, transition to night
	// Else (switch-case), set brightness to time state 
	if (time.time > time.morning_time - time.morning_offset &&
		time.time <= time.morning_time) {
	    double distance_opp = abs(static_cast<double>(time.time) -
						   static_cast<double>(time.morning_time)) / time.morning_offset;
		
	    double distance = (static_cast<double>(time.time) -
						   static_cast<double>(time.morning_time) +
						   static_cast<double>(time.morning_offset)) / time.morning_offset;
	    dark.value = distance_opp*darkness_amount;
		r = distance * static_cast<double>(day_color_r);
		g = distance * static_cast<double>(day_color_g);
		b = distance * static_cast<double>(day_color_b);
	} else if (time.time > time.night_time - time.night_offset &&
		time.time <= time.night_time) {
		double distance_opp = abs(static_cast<double>(time.time) -
						   static_cast<double>(time.night_time)) / time.night_offset;
	    double distance = (static_cast<double>(time.time) -
						   static_cast<double>(time.night_time) +
						   static_cast<double>(time.night_offset)) / time.night_offset;
	    dark.value = distance*darkness_amount;
		r = distance_opp * day_color_r;
		g = distance_opp * day_color_g;
		b = distance_opp * day_color_b;
	} else {
		switch(time.state) {
		case TIME_DAY:
			dark.value = 0;
			r = day_color_r;
			g = day_color_g;
			b = day_color_b;
			break;
		case TIME_NIGHT:
			dark.value = darkness_amount;
			r = 0;
			g = 0;
			b = 0;
			break;
		default:
			break;
		}
	}

	darkness = dark.get();
	
	// Update sun/moon
	bg_light_src.x = 0;
	bg_light_src.y = 0;
	bg_light_src.w = bg_light_w;
	bg_light_src.h = bg_light_h;

	bg_light_dest.x = light_cam_left+(sin(time_over_max * (M_PI*2))*-1)*hor_circle;
	bg_light_dest.y = bg_hills_offset_y+light_cam_top+cos(time_over_max * (M_PI*2))*vert_circle;
	bg_light_dest.w = light_width;
	bg_light_dest.h = light_height;

	bg_moon_src.x = 0;
	bg_moon_src.y = 0;
	bg_moon_src.w = bg_light_w;
	bg_moon_src.h = bg_light_h;

	bg_moon_dest.x = light_cam_left+(sin((time_over_max+.50) * (M_PI*2))*-1)*hor_circle;
	bg_moon_dest.y = bg_hills_offset_y+light_cam_top+cos((time_over_max+.50) * (M_PI*2))*vert_circle;
	bg_moon_dest.w = light_width;
	bg_moon_dest.h = light_height;
}

void Background::render(GraphicsWrapper* renderer) {
	static double cavebg_opacity = 1;
	
	const double transition = 255;

	if (show_cave_background) {
		                  // to         value          divider
		cavebg_opacity += ((transition-cavebg_opacity)/4);
	} else {
		cavebg_opacity += ((0-cavebg_opacity)/4);
	}
	if (cavebg_opacity > transition-1) cavebg_opacity = 255;
	
	int offset = 200;

	// Render sky
	Rect sky{0, 0, win_width, win_height};
	renderer->render_rect(sky, Color{r, g, b, 255});
	
	// Render bg_shine
	renderer->render(bg_shine_src, bg_shine_dest, 13);

	// Render sun/moon
	renderer->render(bg_light_src, bg_light_dest, 20);
	renderer->render(bg_moon_src, bg_moon_dest, 21);

	// Render clouds
	render_repeating(renderer, 14, bg_cloud_offset_x, bg_cloud_offset_y,
					 bg_cloud_w, bg_cloud_h, 60, 40 + offset);

	const int hill_offset = 0;
	
	// Render hills
	render_repeating(renderer, 15, bg_hills_offset_x, bg_hills_offset_y,
					 bg_hills_w, bg_hills_h, 0, hill_offset);

	const int after_hills_top = bg_hills_offset_y+bg_hills_h+hill_offset;
	Rect after{0, after_hills_top, win_width, win_height-after_hills_top};
	renderer->render_rect(after, bg_hills_extend);

	if (cavebg_opacity > 2) {
		int texture = 17;
		renderer->set_opacity(static_cast<int>(cavebg_opacity), texture);
		
		render_repeating(renderer, texture, bg_cave_block_x, bg_cave_block_y,
						 bg_cave_block_w, bg_cave_block_h, 0, 0, true, constants::block_img_size, constants::block_img_size);

		// Reset opacity for future objects
		renderer->set_opacity(255, texture);
	}
}

void Background::render_light(GraphicsWrapper* renderer, Camera& camera) {
	Rect box{0, 0, camera.get_width(), camera.get_height()};
	Color color{10, 10, 10, darkness};
	renderer->render_rect(box, color);
}

void Background::render_repeating(GraphicsWrapper* renderer, int texture, int offset_x, int offset_y, int width,
								  int height, int gap, int top, bool verticle, int src_w, int src_h) {
	if (src_w == -1) src_w = width;
	if (src_h == -1) src_h = height;
	
	const int MAX_X = win_width;
	const int MAX_Y = win_height;
	// Create a grid of tiles as the background
	for (int i = -1; (width+gap)*i < MAX_X+(width+gap); i++) {
		for (int j = -1; verticle ? (height+gap)*j < MAX_Y+(height+gap) : j < 0; j++) {
			Rect src{0, 0, src_w, src_h};
		    Rect block{(offset_x % (width+gap))+((width+gap)*i),
				verticle ? ((offset_y % (height+gap)) + top)+((height+gap)*j) : top+offset_y,
				width, height};

			// Draw the tile
			renderer->render(src, block, texture);
		}
	}
}
