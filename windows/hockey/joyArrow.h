#ifndef _JOYARROW_H_
#define _JOYARROW_H_

// pre-declare classes
class STexture;

class joyArrow {
private:
	float angle;
	STexture* tex;
public:
	joyArrow();
	~joyArrow();

	void setAngle(double angl);
	void render(int x, int y);
};

#endif /* _JOYARROW_H_ */

