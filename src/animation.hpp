#ifndef ANIMATION_HPP
#define ANIMATION_HPP

class Animation {
public:
    Animation() {};
    Animation(int maximum);
    ~Animation();

    int tick();
    int get_frame();
    int get_max_frames();
    void reset(int num = 0);
private:
    int frame;
    int max_frames;
};


#endif // ANIMATION_HPP
