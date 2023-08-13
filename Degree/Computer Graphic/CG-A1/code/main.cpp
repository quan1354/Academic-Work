#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include "Object.h"
#include "xstring.h"

using namespace std;
const int WINDOW_WIDTH = 913;
const int WINDOW_HEIGHT = 600;

void renderWindow();
void drawBackground();
void renderItems();
void drawMusicPart();
void drawMapPart();
void drawChatbotPart();
void drawApplicationPart();
void drawCarStatusPart();
void drawHeader();

Object object = Object();
string text;
int main() {
	// Set window display mode to single window and RGBA color
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	// Set window size
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(0, 0);

	// Set window title
	glutCreateWindow("My Car Dashboard");

	// Load render function
	glutDisplayFunc(renderWindow);

	// Loop Frame Forever, until press ESP key 
	glutMainLoop();
	return 0;

}
void renderWindow() {
	// Set background color
	glClearColor(253 / 255.0f, 220 / 255.0f, 201 / 255.0f, 1.0f);
	// Set Canvas Size
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Set Resolution
	gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
	// Load Display Buffer
	glClear(GL_COLOR_BUFFER_BIT);

	//initialize point variables to auto convert between drawing and screen size
	Point::setDrawingAndScreenSize(3650.0f, 2400.0f, 913.0f, 600.0f);

	drawBackground();
	renderItems();

	// Clear execution command
	glFlush();
	glFinish();
}
void drawBackground() {
	// Music Background
	object.setColor(106, 179, 180);
	object.drawRectangle(Point(34, 2370), Point(1184, 2370), Point(1184, 1809), Point(34, 1809));
	object.setColor(232,232,232);
	object.drawRectangle(Point(64, 2340), Point(1154, 2340), Point(1154, 1839), Point(64, 1839));
	// Map Background
	object.setColor(206, 166, 45);
	object.drawRectangle(Point(34, 1785), Point(1184, 1785), Point(1184, 185), Point(34, 185));
	object.setColor(204, 204, 204);
	object.drawRectangle(Point(64, 1755), Point(1154, 1755), Point(1154, 215), Point(64, 215));
	// Bot Background
	object.setColor(97, 150, 204);
	object.drawRectangle(Point(1192, 949), Point(2457, 949), Point(2457, 185), Point(1192, 185));
	object.setColor(210, 217, 255);
	object.drawRectangle(Point(1222, 919), Point(2427, 919), Point(2427, 215), Point(1222, 215));
	// Car Status Background
	object.setColor(218, 72, 250);
	object.drawRectangle(Point(2470, 2370), Point(3616, 2370), Point(3616, 185), Point(2470, 185));
	object.setColor(137, 210, 248);
	object.drawRectangle(Point(2500, 2340), Point(3586, 2340), Point(3586, 215), Point(2500, 215));
	
	// Car Window Controller Background
	object.setColor(97, 150, 204);
	object.drawRectangle(Point(1361, 2237), Point(1805, 2356), Point(1805, 2007), Point(1220, 2007));
	object.setColor(201, 217, 255);
	object.drawRectangle(Point(1391, 2207), Point(1772, 2318), Point(1772, 2037), Point(1274, 2037));
	object.setColor(97, 150, 204);
	object.drawRectangle(Point(1848, 2356), Point(2291, 2238), Point(2432, 2011), Point(1848, 2011));
	object.setColor(201, 217, 255);
	object.drawRectangle(Point(1880, 2318), Point(2272, 2215), Point(2379, 2042), Point(1880, 2042));
	
	// Hearder Seperater Line
	object.setColor(155, 147, 0);
	object.drawLine(Point(0,152), Point(3650,152), 7);

	// - Navigation Background
	object.setColor(18, 0, 255);
	object.drawPolygon(Point(1550, 1888), Point(110, 0).getConvertX(), 360, 0, 2.0f, 0.8f);
	object.drawPolygon(Point(2110, 1888), Point(110, 0).getConvertX(), 360, 0, 2.0f, 0.8f);
	object.setColor(180, 251, 247);
	object.drawPolygon(Point(1550, 1888), Point(110, 0).getConvertX(), 360, 0, 1.7f, 0.6f);
	object.drawPolygon(Point(2110, 1888), Point(110, 0).getConvertX(), 360, 0, 1.7f, 0.6f);

	// - The 6 application Background 
	object.setColor(244, 92, 5);
	object.drawRectangle(Point(1221,1367),Point(1592,1367),Point(1592,996),Point(1221,996));
	object.setColor(180, 251, 247);
	object.drawRectangle(Point(1241, 1347), Point(1572, 1347), Point(1572, 1016), Point(1241, 1016));
	
	object.setColor(244, 92, 5);
	object.drawRectangle(Point(1640, 1367), Point(2013, 1367), Point(2013, 996), Point(1640, 996));
	object.setColor(180, 251, 247);
	object.drawRectangle(Point(1660, 1347), Point(1997, 1347), Point(1997, 1016), Point(1660, 1016));
	
	object.setColor(244, 92, 5);
	object.drawRectangle(Point(2064, 1367), Point(2436, 1367), Point(2436, 996), Point(2064, 996));
	object.setColor(180, 251, 247);
	object.drawRectangle(Point(2084, 1347), Point(2416, 1347), Point(2416, 1016), Point(2084, 1016));

	object.setColor(244, 92, 5);
	object.drawRectangle(Point(1221, 1785), Point(1592, 1785), Point(1592, 1415), Point(1221, 1415));
	object.setColor(180, 251, 247);
	object.drawRectangle(Point(1241, 1765), Point(1572, 1765), Point(1572, 1435), Point(1241, 1435));

	object.setColor(244, 92, 5);
	object.drawRectangle(Point(1640, 1785), Point(2013, 1785), Point(2013, 1415), Point(1640, 1415));
	object.setColor(180, 251, 247);
	object.drawRectangle(Point(1660, 1765), Point(1993, 1765), Point(1993, 1435), Point(1660, 1435));

	object.setColor(244, 92, 5);
	object.drawRectangle(Point(2064, 1785), Point(2436, 1785), Point(2436, 1415), Point(2064, 1415));
	object.setColor(180, 251, 247);
	object.drawRectangle(Point(2084, 1765), Point(2416, 1765), Point(2416, 1435), Point(2084, 1435));
}
void renderItems() {
	drawHeader();
	drawMusicPart();
	drawMapPart();
	drawChatbotPart();
	drawApplicationPart();
	drawCarStatusPart();
}

void drawHeader() {
	
	// Phone Line of strength
	object.setColor(156, 100, 27);
	object.drawLine(Point(153, 112), Point(153, 70), 3);
	object.drawLine(Point(180, 112), Point(180, 52), 3);
	object.drawLine(Point(209, 112), Point(209, 27), 3);

	object.setColor(156,100,27);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	text = "4G";
	object.setColor(156, 100, 27);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	object.drawText(text.data(), text.size(), Point(287, 90), GLUT_BITMAP_HELVETICA_18);

	text = "TERA";
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	object.drawText(text.data(), text.size(), Point(1711, 90), GLUT_BITMAP_HELVETICA_18);

	text = "16 FEBRUARY 2013   12.00p.m.";
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	object.drawText(text.data(), text.size(), Point(2580, 90), GLUT_BITMAP_HELVETICA_18);
}

void drawMusicPart() {
	// Lines
	object.setColor(0, 0, 0);
	object.drawLine(Point(490, 2340), Point(490, 1840), 8);
	object.drawLine(Point(540, 2215), Point(1125, 2215), 8);
	object.setColor(255, 255, 255);
	object.drawLine(Point(545, 2215), Point(780, 2215), 6);
	 
	// Song Broadcast background 
	object.setColor(106, 179, 180);
	object.drawRectangle(Point(87, 2260), Point(455, 2260), Point(455, 1900), Point(87, 1900));
	//object.setColor(232, 232, 232);
	//object.drawRectangle(Point(97, 2256), Point(443, 2256), Point(443, 1912), Point(97, 1912));
	for (float i = 27; i < 110; ++i) {
		for (float j = 40; j < 120; ++j) {
			glColor3f(1.0- ((j - 40) * 1.0f / 120),1,0);
			object.drawPoint(i,j, 1);
		}
	}

	// Minecraft Soil Icon
	object.setColor(184, 146, 96);
	object.drawRectangle(Point(178, 2148), Point(274, 2176), Point(274, 2063), Point(178, 2034));
	object.drawRectangle(Point(274, 2176), Point(370, 2148), Point(370, 2034), Point(274, 2063));
	object.setColor(120, 95, 63);
	object.drawLine(Point(274, 2063), Point(274, 2176), 2);
	object.setColor(46, 215, 58);
	object.drawRectangle(Point(178, 2034), Point(274, 2063), Point(370, 2034), Point(260, 2007));
	 
	// Triangle Arrow Button
	object.setColor(52, 147, 236);
	object.drawPolygon(Point(845, 2023), Point(54, 0).getConvertX(), 3, 0.0f, 1.0f, 1.0f);
	object.drawPolygon(Point(600, 2023), Point(54, 0).getConvertX(), 3, 1.0f, 1.0f, 1.0f); 
	object.drawPolygon(Point(650, 2023), Point(54, 0).getConvertX(), 3, 1.0f, 1.0f, 0.9f);
	object.drawPolygon(Point(1067, 2023), Point(54, 0).getConvertX(), 3, 0.0f, 1.0f, 1.0f);
	object.drawPolygon(Point(1017, 2023), Point(54, 0).getConvertX(), 3, 0.0f, 1.0f, 0.9f);

	object.setColor(0,0,0);
	text = "Minecraft Song";
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	object.drawText(text.data(), text.size(), Point(680, 1930), GLUT_BITMAP_HELVETICA_12);
}

void drawMapPart() {
	// Search Icon
	object.setColor(255, 141, 58);
	object.drawPolygon(Point(179,333),Point(100.0,0).getConvertX(),360);
	object.drawPolygon(Point(1041, 1633), Point(100.0, 0).getConvertX(), 360);
	object.setColor(255, 255, 255);
	object.drawPolygon(Point(173, 339), Point(40.0, 0).getConvertX(), 360);
	object.setColor(255, 141, 58);
	object.drawPolygon(Point(173, 339), Point(20.0, 0).getConvertX(), 360);
	object.setColor(255, 255, 255);
	object.drawLine(Point(203, 363), Point(220, 377), 5);

	// Location Icon
	object.setColor(255, 255, 255);
	object.drawTriangle(Point(958, 1645), Point(1026, 1641), Point(1089, 1583));
	object.drawTriangle(Point(1020, 1641), Point(1023, 1707), Point(1089, 1580));

	// Draw Navigation Arrow and Pointer Icon
	object.setColor(52, 147, 236);
	object.drawTriangle(Point(296, 1514), Point(120, 1582), Point(222, 1602));
	object.drawTriangle(Point(222, 1602), Point(262, 1695), Point(296, 1514));

	object.setColor(77, 76, 76);
	object.drawLine(Point(960, 454), Point(960, 542), 5);
	object.setColor(225, 33, 33);
	object.drawPolygon(Point(964, 454), Point(25, 0).getConvertX(), 360, 0, 1.0f, 1.0f);

	// Draw House
	object.setColor(54, 251, 47);
	object.drawRectangle(Point(600, 1564), Point(752, 1564), Point(752, 1416), Point(600, 1416));
	object.drawRectangle(Point(730, 1168), Point(884, 1168), Point(884, 1020), Point(730, 1020));
	object.drawRectangle(Point(468, 510), Point(620, 510), Point(620, 362), Point(468, 362));
	object.drawRectangle(Point(116, 1050), Point(268, 1050), Point(268, 900), Point(116, 900));

	object.setColor(255, 0, 0);
	object.drawPolygon(Point(675, 1375), Point(80, 0).getConvertX(), 3, 12.0f, 1.5f, 1.0f);
	object.drawPolygon(Point(810, 980), Point(80, 0).getConvertX(), 3, 12.0f, 1.5f, 1.0f);
	object.drawPolygon(Point(545, 324), Point(80, 0).getConvertX(), 3, 12.0f, 1.5f, 1.0f);
	object.drawPolygon(Point(193, 860), Point(80, 0).getConvertX(), 3, 12.0f, 1.5f, 1.0f);

	// Draw Love
	object.setColor(244, 103, 103);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	object.rotate(Point(610, 894), 190.0f);
	object.drawPolygon(Point(610, 894), Point(55, 0).getConvertX(), 360, 190.0f, 1.0f, 2.0f);
	object.rotate(Point(680, 845), 250.0f);
	object.drawPolygon(Point(680, 845), Point(55, 0).getConvertX(), 360, 250.0f, 1.0f, 2.0f);
	object.finishRotation();
	
	object.setColor(204, 204, 204);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	object.rotate(Point(610, 894), 190.0f);
	object.drawPolygon(Point(610, 894), Point(38, 0).getConvertX(), 360, 190.0f, 1.0f, 2.0f);
	object.rotate(Point(680, 845), 250.0f);
	object.drawPolygon(Point(680, 845), Point(38, 0).getConvertX(), 360, 250.0f, 1.0f, 2.0f);
	object.finishRotation();

	// Draw Line
	object.setColor(66, 151, 146);
	object.drawLine(Point(267, 743), Point(420, 563), 5);
	object.drawLine(Point(420, 563), Point(648, 547), 5);
	object.drawLine(Point(648, 547), Point(718, 900), 5);

	object.drawLine(Point(566, 1136), Point(1095, 1415), 5);
	object.drawLine(Point(839, 1276), Point(1055, 1019), 5);

	object.setColor(244, 103, 103);
	object.drawLine(Point(291, 1517), Point(402, 1384), 5);
	object.drawLine(Point(402, 1384), Point(401, 1222), 5);
	object.drawLine(Point(401, 1222), Point(549, 1150), 5);
	object.drawLine(Point(549, 1150), Point(771, 823), 5);
	object.drawLine(Point(771, 823), Point(918, 764), 5);
	object.drawLine(Point(918, 764), Point(902, 611), 5);
	object.drawLine(Point(902, 611), Point(962, 535), 5);

	// Draw Text
	object.setColor(0, 0, 0);
	text = "School";
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	object.drawText(text.data(), text.size(), Point(593, 280), GLUT_BITMAP_HELVETICA_10);

	text = "Toilet";
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	object.drawText(text.data(), text.size(), Point(135, 1090), GLUT_BITMAP_HELVETICA_10);

	text = "Girlfriend house";
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	object.drawText(text.data(), text.size(), Point(833, 385), GLUT_BITMAP_HELVETICA_10);

	text = "Home";
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	object.drawText(text.data(), text.size(), Point(840, 920), GLUT_BITMAP_HELVETICA_10);

	text = "Bank";
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	object.drawText(text.data(), text.size(), Point(660, 1600), GLUT_BITMAP_HELVETICA_10);
}

void drawApplicationPart() {
	// Windows controller Icon
	object.setColor(52, 147, 236);
	object.drawRectangle(Point(1500, 2164), Point(1610, 2165), Point(1610, 2114), Point(1500, 2114));
	object.drawRectangle(Point(2017, 2164), Point(2132, 2165), Point(2132, 2114), Point(2017, 2114));
	object.drawRectangle(Point(1482, 2153), Point(1595, 2154), Point(1595, 2128), Point(1482, 2128));
	object.drawRectangle(Point(2132, 2153), Point(2150, 2153), Point(2150, 2127), Point(2132, 2127));

	// Leftside updown
	object.setColor(0, 0, 0);
	object.drawLine(Point(1514,2086), Point(1550,2054), 3);
	object.drawLine(Point(1550, 2054), Point(1591, 2087),3);
	object.drawLine(Point(1514, 2206), Point(1551, 2245), 3);
	object.drawLine(Point(1551, 2245), Point(1591, 2206), 3);

	// Rightside updown
	object.drawLine(Point(2034, 2086), Point(2074, 2054), 3);
	object.drawLine(Point(2074, 2054), Point(2112, 2087), 3);
	object.drawLine(Point(2034, 2210), Point(2074, 2245), 3);
	object.drawLine(Point(2074, 2245), Point(2112, 2210), 3);

	// Leftside leftright 
	object.drawLine(Point(1973, 2105), Point(1932, 2143), 3);
	object.drawLine(Point(1932, 2143), Point(1973, 2181), 3);
	object.drawLine(Point(2175, 2105), Point(2215, 2143), 3);
	object.drawLine(Point(2215, 2143), Point(2175, 2181), 3);
	
	// Rightside leftright
	object.drawLine(Point(1452, 2105), Point(1413, 2143), 3);
	object.drawLine(Point(1413, 2143), Point(1452, 2181), 3);
	object.drawLine(Point(1654, 2105), Point(1694, 2143), 3);
	object.drawLine(Point(1694, 2143), Point(1654, 2181), 3);

	// App of left right Navigation Button
	object.setColor(18, 0, 255);
	object.drawPolygon(Point(1509, 1891), Point(30, 0).getConvertX(), 3, 1.0f, 1.5f, 1.5f);
	object.drawPolygon(Point(2158, 1891), Point(30, 0).getConvertX(), 3, 0.0f, 1.5f, 1.5f);
	object.drawRectangle(Point(1525, 1910), Point(1632, 1910), Point(1632, 1872), Point(1525, 1872));
	object.drawRectangle(Point(2035, 1910), Point(2135, 1910), Point(2135, 1872), Point(2035, 1872));

	// Music Application
	object.setColor(52, 147, 236);
	object.drawLine(Point(1334, 1260), Point(1334, 1103), 5);
	object.drawLine(Point(1334, 1103), Point(1484, 1091), 5);
	object.drawLine(Point(1484, 1091), Point(1483, 1250), 5);
	object.drawPolygon(Point(1321, 1254), Point(24, 0).getConvertX(), 360, 0, 1.0f, 1.0f); 
	object.drawPolygon(Point(1469, 1250), Point(24, 0).getConvertX(), 360, 0, 1.0f, 1.0f); 

	// YoutTube Application
	object.drawRectangle(Point(1749, 1226), Point(1922, 1226), Point(1922, 1114), Point(1749, 1114));
	object.drawRectangle(Point(1773, 1266), Point(1895, 1266), Point(1895, 1233), Point(1773, 1233));
	object.setColor(255, 255, 255);
	object.drawPolygon(Point(1837, 1170), Point(30, 0).getConvertX(), 3, 0.0f, 1.5f, 1.5f);

	// Telephone Application
	object.setColor(52, 147, 236);
	object.drawRectangle(Point(2124, 1127), Point(2285, 1291), Point(2323, 1264), Point(2163, 1101));
	object.drawRectangle(Point(2162, 1100), Point(2202, 1141), Point(2242, 1110), Point(2203, 1069));
	object.drawRectangle(Point(2273, 1232), Point(2311, 1273), Point(2356, 1236), Point(2317, 1195));

	// Chat Application
	object.drawPolygon(Point(1414, 1598), Point(100, 0).getConvertX(), 360, 0.0f, 1.0f, 0.8f);
	object.drawPolygon(Point(1334, 1665), Point(30, 0).getConvertX(), 3, 180, 1.5f, 1.5f);
	object.setColor(232, 232, 232);
	object.drawPolygon(Point(1360, 1600), Point(15, 0).getConvertX(), 360, 0.0f, 1.0f, 1.0f);
	object.drawPolygon(Point(1422, 1600), Point(15, 0).getConvertX(), 360, 0.0f, 1.0f, 1.0f);
	object.drawPolygon(Point(1480, 1600), Point(15, 0).getConvertX(), 360, 0.0f, 1.0f, 1.0f);

	// Map Application
	object.setColor(52, 147, 236);
	object.drawTriangle(Point(1922, 1527), Point(1708, 1575), Point(1808, 1618));
	object.drawTriangle(Point(1808, 1618), Point(1823, 1724), Point(1922, 1527));

	// Lock Application
	object.drawPolygon(Point(2247, 1527), Point(30, 0).getConvertX(), 360, 0.0f, 2.0f, 2.0f);
	object.setColor(255, 255, 255);
	object.drawPolygon(Point(2247, 1527), Point(30, 0).getConvertX(), 360, 0.0f, 1.0f, 1.0f);
	object.setColor(52, 147, 236);
	object.drawRectangle(Point(2162, 1695), Point(2330, 1695), Point(2330, 1526), Point(2162, 1526));
	object.setColor(255, 255, 255);
	object.drawPolygon(Point(2247, 1599), Point(30, 0).getConvertX(), 360, 0.0f, 1.0f, 1.0f);
	object.drawRectangle(Point(2234, 1650), Point(2257, 1650), Point(2257, 1620), Point(2234, 1620));
}

void drawChatbotPart() {
	// Battery
	object.setColor(255, 255, 255);
	object.drawRectangle(Point(1316, 343), Point(1487, 343), Point(1487, 294), Point(1316, 294));
	object.setColor(36, 255, 0);
	object.drawRectangle(Point(1330, 334), Point(1466, 334), Point(1466, 303), Point(1330, 303));

	// Doraemon
	object.setColor(52, 147, 236);
	// move bot of head up by 20 pixel on y-axis
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f,25.0f,0.0f);
	object.drawPolygon(Point(1811, 700), Point(70, 0).getConvertX(), 360, 0.0f, 4.5f, 4.0f);
	glLoadIdentity();
	object.setColor(255, 255, 255);
	object.drawPolygon(Point(1811, 650), Point(70, 0).getConvertX(), 360, 0.0f, 3.6f, 2.6f);

	// Mouth
	object.setColor(225, 33, 33);
	object.drawPolygon(Point(1811, 660), Point(70, 0).getConvertX(), 360, 0.0f, 2.0f, 2.0f);
	object.setColor(255, 255, 255);
	object.drawRectangle(Point(1660, 701), Point(1964, 701), Point(1954, 520), Point(1666, 520));
	object.setColor(252, 152, 152);
	object.drawPolygon(Point(1811, 750), Point(30, 0).getConvertX(), 360, 0.0f, 1.5f, 1.0f);

	// Lines
	object.setColor(0, 0, 0);
	object.drawLine(Point(1810, 580), Point(1810, 700), 3);
	object.drawLine(Point(1588, 559), Point(1763, 615), 3);
	object.drawLine(Point(1590, 639), Point(1765, 637), 3);
	object.drawLine(Point(1587, 715), Point(1761, 659), 3);
	object.drawLine(Point(1862, 659), Point(2037, 716), 3);
	object.drawLine(Point(1865, 638), Point(2040, 636), 3);
	object.drawLine(Point(1861,615), Point(2034, 559), 3);

	// Nose
	object.setColor(255, 0, 0);
	object.drawPolygon(Point(1810, 570), Point(30, 0).getConvertX(), 360, 0.0f, 1.0f, 1.0f);
	object.setColor(255, 255, 255);
	object.drawPolygon(Point(1808, 570), Point(10, 0).getConvertX(), 360, 0.0f, 1.0f, 1.0f);

	// Eyes
	object.setColor(0, 0, 0);
	object.drawPolygon(Point(1734, 470), Point(40, 0).getConvertX(), 360, 0.0f, 1.0f, 1.5f);
	object.setColor(255, 255, 255);
	object.drawPolygon(Point(1734, 470), Point(35, 0).getConvertX(), 360, 0.0f, 1.0f, 1.5f);
	object.setColor(0, 0, 0);
	object.drawPolygon(Point(1734, 470), Point(20, 0).getConvertX(), 360, 0.0f, 1.0f, 1.5f);

	object.setColor(0, 0, 0);
	object.drawPolygon(Point(1894, 470), Point(40, 0).getConvertX(), 360, 0.0f, 1.0f, 1.5f);
	object.setColor(255, 255, 255);
	object.drawPolygon(Point(1894, 470), Point(35, 0).getConvertX(), 360, 0.0f, 1.0f, 1.5f);
	object.setColor(0, 0, 0);
	object.drawPolygon(Point(1894, 470), Point(20, 0).getConvertX(), 360, 0.0f, 1.0f, 1.5f);

	object.setColor(0, 0, 0);
	text = "123153 km";
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	object.drawText(text.data(), text.size(), Point(2050, 320), GLUT_BITMAP_HELVETICA_18);
}

void drawCarStatusPart() {
	// Rectangle shape Background
	object.setColor(0, 12, 255);
	object.drawRectangle(Point(2847, 2237), Point(3041, 2238), Point(3041, 2034), Point(2847, 2034));
	object.setColor(254, 187, 100);
	object.drawRectangle(Point(2533, 1912), Point(3561, 1912), Point(3561, 1350), Point(2533, 1350));

	// Coolant Level Warning Light Meter
	object.setColor(0, 0, 0);
	object.drawLine(Point(2613,1595), Point(2613,1536),5);
	object.drawLine(Point(2613, 1566), Point(2849, 1564), 5);
	object.drawLine(Point(2849, 1595), Point(2849, 1536), 5);
	object.setColor(18, 0, 255);
	object.drawLine(Point(2682, 1595), Point(2682, 1536), 5);

	// Coolant Level Warning Light Icon
	object.setColor(255, 255, 255);
	object.drawPolygon(Point(2717, 1775), Point(20, 0).getConvertX(), 360, 0.0f, 1.0f, 1.0f);
	object.drawLine(Point(2717, 1755), Point(2717, 1675), 5);
	object.drawLine(Point(2711, 1703), Point(2761, 1703), 5);
	object.drawLine(Point(2711, 1735), Point(2761, 1735), 5);

	// top wave
	object.drawLine(Point(2657, 1816), Point(2682, 1801), 3);
	object.drawLine(Point(2676, 1801), Point(2699, 1816), 3);
	object.drawLine(Point(2699, 1816), Point(2718, 1801), 3);
	object.drawLine(Point(2713, 1801), Point(2735, 1816), 3);
	object.drawLine(Point(2731, 1816), Point(2751, 1801), 3);
	object.drawLine(Point(2747, 1801), Point(2770, 1816), 3);

	// bottom wave
	object.drawLine(Point(2657, 1837), Point(2682, 1822), 3);
	object.drawLine(Point(2676, 1822), Point(2699, 1837), 3);
	object.drawLine(Point(2699, 1837), Point(2718, 1822), 3);
	object.drawLine(Point(2713, 1822), Point(2735, 1837), 3);
	object.drawLine(Point(2731, 1837), Point(2751, 1822), 3);
	object.drawLine(Point(2747, 1822), Point(2770, 1837), 3);

	// Car fuel level Meter
	object.setColor(0, 0, 0);
	object.drawLine(Point(2983,1595), Point(2983,1536),5);
	object.drawLine(Point(2983, 1566), Point(3222, 1564), 5);
	object.drawLine(Point(3225, 1595), Point(3225, 1536), 5);
	object.setColor(18, 0, 255);
	object.drawLine(Point(3103, 1595), Point(3103, 1536), 5);

	// Car fuel Icon 
	object.setColor(255, 255, 255);
	object.drawRectangle(Point(3055, 1818), Point(3138, 1818), Point(3138, 1680), Point(3055, 1680)); 
	object.drawRectangle(Point(3041, 1829), Point(3154, 1829), Point(3154, 1816), Point(3041, 1816));
	object.drawLine(Point(3131, 1794), Point(3176, 1794), 5);
	object.drawLine(Point(3172, 1799), Point(3172, 1712), 5);
	object.drawLine(Point(3172, 1721), Point(3155, 1721), 5);
	object.setColor(254, 187, 100);
	object.drawRectangle(Point(3070, 1735), Point(3123, 1735), Point(3123, 1700), Point(3070, 1700));
	
	// Battery Light
	object.setColor(255, 255, 255);
	object.drawRectangle(Point(3357, 1640), Point(3487, 1640), Point(3487, 1553), Point(3357, 1553));
	object.drawRectangle(Point(3372, 1553), Point(3396, 1553), Point(3396, 1526), Point(3372, 1526));
	object.drawRectangle(Point(3446, 1553), Point(3471, 1553), Point(3471, 1525), Point(3446, 1525));
	object.setColor(0, 0, 0);
	object.drawLine(Point(3375, 1600), Point(3396, 1600), 2);
	object.drawLine(Point(3448, 1600), Point(3472, 1600), 2);
	object.drawLine(Point(3460, 1588), Point(3460, 1612), 2);
	
	// Seat belt signal
	object.setColor(255, 0, 0);
	object.drawPolygon(Point(3419, 1718), Point(30, 0).getConvertX(), 360, 0.0f, 1.0f, 1.0f);
	object.drawPolygon(Point(3419, 1783), Point(30, 0).getConvertX(), 360, 0.0f, 1.0f, 1.5f);
	object.drawLine(Point(3359, 1817), Point(3484, 1714), 5);
	object.drawRectangle(Point(3381, 1847), Point(3453, 1847), Point(3453, 1823), Point(3381, 1823));

	// Draw Cat Car
	object.setColor(184, 146, 96);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 

	// Legs
	object.rotate(Point(2679, 1137), -45.0f);
	object.drawPolygon(Point(2679, 1137), Point(50, 0).getConvertX(), 360, -45.0f, 1.0f, 2.0f);
	object.finishRotation();
	object.rotate(Point(2879, 1143), -45.0f);
	object.drawPolygon(Point(2879, 1143), Point(50, 0).getConvertX(), 360, -45.0f, 1.0f, 2.0f);
	object.finishRotation();

	object.drawPolygon(Point(2994, 1120), Point(50, 0).getConvertX(), 360, 0.0f, 1.0f, 2.0f);
	object.drawPolygon(Point(3088, 1120), Point(50, 0).getConvertX(), 360, 0.0f, 1.0f, 2.0f);
	object.drawPolygon(Point(3185, 1120), Point(50, 0).getConvertX(), 360, 0.0f, 1.0f, 2.0f);
	object.drawPolygon(Point(3275, 1120), Point(50, 0).getConvertX(), 360, 0.0f, 1.0f, 2.0f);
	object.drawPolygon(Point(3365, 1120), Point(50, 0).getConvertX(), 360, 0.0f, 1.0f, 2.0f);
	object.drawPolygon(Point(3455, 1120), Point(50, 0).getConvertX(), 360, 0.0f, 1.0f, 2.0f);

	object.setColor(106, 79, 64);
	object.rotate(Point(2666, 1157), -45.0f);
	object.drawPolygon(Point(2666, 1157), Point(15, 0).getConvertX(), 360, -45.0f, 2.0f, 1.0f);
	object.finishRotation();
	object.rotate(Point(2860, 1170), -45.0f);
	object.drawPolygon(Point(2860, 1170), Point(15, 0).getConvertX(), 360, -45.0f, 2.0f, 1.0f);
	object.finishRotation();

	object.drawPolygon(Point(2991, 1159), Point(15, 0).getConvertX(), 360, 0.0f, 2.0f, 1.0f);
	object.drawPolygon(Point(3093, 1159), Point(15, 0).getConvertX(), 360, 0.0f, 2.0f, 1.0f);
	object.drawPolygon(Point(3190, 1159), Point(15, 0).getConvertX(), 360, 0.0f, 2.0f, 1.0f);
	object.drawPolygon(Point(3274, 1159), Point(15, 0).getConvertX(), 360, 0.0f, 2.0f, 1.0f);
	object.drawPolygon(Point(3365, 1159), Point(15, 0).getConvertX(), 360, 0.0f, 2.0f, 1.0f);
	object.drawPolygon(Point(3457, 1159), Point(15, 0).getConvertX(), 360, 0.0f, 2.0f, 1.0f);

	// Body
	object.setColor(106, 79, 64);
	object.drawRectangle(Point(2905, 1096), Point(3514, 1096), Point(3388, 842), Point(2905, 842));
	object.setColor(184, 146, 96);
	object.drawRectangle(Point(2917, 1084), Point(3502, 1084), Point(3376, 854), Point(2917, 854));
	object.setColor(106, 79, 64);
	object.drawRectangle(Point(3010, 1031), Point(3403, 1031), Point(3403, 914), Point(3010, 914));
	object.setColor(212, 190, 104);
	object.drawRectangle(Point(3018, 1023), Point(3395, 1023), Point(3395, 922), Point(3018, 922));
	object.setColor(106, 79, 64);
	object.drawLine(Point(3205, 915), Point(3205, 1025), 2);

	// Head
	object.setColor(106, 79, 64);
	object.drawPolygon(Point(2801, 936), Point(50, 0).getConvertX(), 360, 0.0f, 4.0f, 4.0f);
	object.setColor(184, 146, 96);
	object.drawPolygon(Point(2801, 936), Point(46, 0).getConvertX(), 360, 0.0f, 4.0f, 4.0f);

	// Mouth
	object.setColor(255, 255, 255);
	object.drawPolygon(Point(2792, 959), Point(40, 0).getConvertX(), 360, 0.0f, 3.0f, 3.0f);
	object.setColor(184, 146, 96);
	object.drawRectangle(Point(2670, 960), Point(2920, 960), Point(2920, 810), Point(2670, 810));
	object.setColor(106, 79, 64);
	object.drawPolygon(Point(2797, 773), Point(40, 0).getConvertX(), 6, 0.0f, 2.0f, 1.0f);

	// Nose
	object.setColor(135, 68, 44);
	object.drawPolygon(Point(2782, 918), Point(10, 0).getConvertX(), 360, 0.0f, 2.0f, 1.0f);

	// Eyes
	object.setColor(212, 190, 104);
	object.drawPolygon(Point(2722, 859), Point(20, 0).getConvertX(), 360, 0.0f, 2.0f, 2.0f);
	object.drawPolygon(Point(2860, 859), Point(20, 0).getConvertX(), 360, 0.0f, 2.0f, 2.0f);
	object.setColor(0, 0, 0);
	object.drawPolygon(Point(2706, 859), Point(10, 0).getConvertX(), 360, 0.0f, 1.0f, 2.0f);
	object.drawPolygon(Point(2842, 859), Point(10, 0).getConvertX(), 360, 0.0f, 1.0f, 2.0f);

	// beard
	object.setColor(106, 79, 64);
	object.drawPolygon(Point(2958, 873), Point(30, 0).getConvertX(), 3, -120.0f, 1.0f, 1.0f);
	object.drawPolygon(Point(2972, 922), Point(30, 0).getConvertX(), 3, 45.0f, 1.0f, 1.0f);
	object.drawPolygon(Point(2633, 885), Point(30, 0).getConvertX(), 3, 0.0f, 1.0f, 1.0f);
	object.drawPolygon(Point(2624, 937), Point(30, 0).getConvertX(), 3, -10.0f, 1.0f, 1.0f);

	// Ear
	object.drawPolygon(Point(2912, 748), Point(40, 0).getConvertX(), 3, -20.0f, 2.0f, 2.0f);
	object.drawPolygon(Point(2670, 763), Point(40, 0).getConvertX(), 3, 0.0f, 2.0f, 2.0f);

	// Toothe Lines
	object.setColor(0, 0, 0);
	object.drawLine(Point(2693, 1000), Point(2893, 1000), 2);
	object.drawLine(Point(2722, 960), Point(2722, 1050), 2);
	object.drawLine(Point(2772, 960), Point(2772, 1070), 2);
	object.drawLine(Point(2816, 960), Point(2816, 1070), 2);
	object.drawLine(Point(2860, 960), Point(2860, 1050), 2);

	// Draw Text
	text = "45";
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	object.drawText(text.data(), text.size(), Point(3000, 338), GLUT_BITMAP_HELVETICA_18);
	text = "Mph";
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	object.drawText(text.data(), text.size(), Point(2980, 500), GLUT_BITMAP_HELVETICA_18);
	text = "N";
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	object.drawText(text.data(), text.size(), Point(2630, 2170), GLUT_BITMAP_HELVETICA_18);
	text = "P";
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	object.drawText(text.data(), text.size(), Point(3200, 2170), GLUT_BITMAP_HELVETICA_18);
	text = "R";
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	object.drawText(text.data(), text.size(), Point(3400, 2170), GLUT_BITMAP_HELVETICA_18);
	object.setColor(255,255,255);
	text = "D";
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	object.drawText(text.data(), text.size(), Point(2920, 2170), GLUT_BITMAP_HELVETICA_18);
}