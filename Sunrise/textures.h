#pragma once

enum class TextureName {
	TEST1,
	TEST2,
	TEST3,
	TEST4,
	TEST5
};

void loadTextures();

void drawTexture(int, int, TextureName);

//Texture mergeTextures(struct[](idTexture, xpos, ypos) //����� �� �������� ������ ���� ������ ���

//int _loadTexture(TextureNames);