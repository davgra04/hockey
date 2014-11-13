#ifndef _JOYARROW_H_
#define _JOYARROW_H_

// pre-declare classes
class LTexture;

class joyArrow {
private:
	float angle;
	LTexture* tex;
public:
	joyArrow();
	~joyArrow();

	void setAngle(double angl);
	void render(int x, int y);
};

#endif /* _JOYARROW_H_ */

