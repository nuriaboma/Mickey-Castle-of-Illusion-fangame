#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"


using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	int tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;

	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;

	getline(fin, line); // mida mapa
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;

	getline(fin, line); // mida tiles i blocs
	sstream.clear();
	sstream.str(line);
	sstream >> tileSize >> blockSize;

	getline(fin, line); // archiu
	sstream.clear();
	sstream.str(line);
	sstream >> tilesheetFile;

	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);

	getline(fin, line);
	sstream.clear();
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;

	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);


	map = new int[mapSize.x * mapSize.y];

	for (int j = 0; j < mapSize.y; ++j)
	{
		for (int i = 0; i < mapSize.x; ++i)
		{
			fin >> tile;
			map[j * mapSize.x + i] = tile;
		}
	}

	fin.close();
	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	nTiles = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if (tile == 42 || tile == 80 || tile == 77 || tile == 78 || tile == 79 || tile == 76) { //Object
				glm::vec2 objectPosition = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				int objectType = 0;

				if (tile == 42) //rock no detroy
					objectType = 1;
				else if (tile == 80) //rock destroy
					objectType = 0;
				else if (tile == 77) // chest
					objectType = 2;
				else if (tile == 78) // cake
					objectType = 3;
				else if (tile == 79) // coin
					objectType = 4;
				else if (tile == 76) // gema
					objectType = 5;

				objectInfo obj = { objectPosition, objectType };
				objects.push_back(obj);
				map[j * mapSize.x + i] = 6;
				tile = 6;
			}
			if(tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile-1)%tilesheetSize.x) / tilesheetSize.x, float((tile-1)/tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		int tile = map[y*mapSize.x + x];
		if (esColisio(tile) || collisionWithRock(pos, size))
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		int tile = map[y*mapSize.x + x];
		if (esColisio(tile) || collisionWithRock(pos, size))
			return true;
	}
	
	return false;
}


bool TileMap::esColisio(int t) const
{
	if (t <= 5 || t == 61 )
		return true;

	return false;
}

bool TileMap::collisionTerra(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		int tile = map[y * mapSize.x + x];
		if (esColisio(tile))
		{
			return true;		
		}
	}
	return false;
}


bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, const glm::ivec2 &sizeSprite) const
{
	int x0, x1, y;
	
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for(int x=x0; x<=x1; x++)
	{
		int tile = map[y*mapSize.x + x];
		if(esColisio(tile) || tile == 23 || collisionWithRock(pos, size))
		{
			if(pos.y - tileSize * y + size.y <= 8)
			{
				*posY = tileSize * y - sizeSprite.y;
				return true;
			}
		}
	}
	
	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = pos.y / tileSize;

	for (int x = x0; x <= x1; x++)
	{
		int tile = map[y*mapSize.x + x];
		if (esColisio(tile) || collisionWithRock(pos, size))
		{
			if (*posY - tileSize * (y + 1) <= 8)
			{
				*posY = tileSize * (y);
				return true;
			}
		}
	}

	return false;
}

bool TileMap::collisionWithRock(const glm::ivec2 &pos, const glm::ivec2 &size) const {
	for (const auto& obj : objects) {
		if (obj.type != 3 && obj.type != 4)
		{
			glm::ivec2 rockSize(32, 32);
			glm::vec2 rockPos = obj.position;
			if (pos.x < rockPos.x + rockSize.x && pos.x + size.x > rockPos.x &&
				pos.y < rockPos.y + rockSize.y && pos.y + size.y > rockPos.y) {
				return true;
			}
		}
	}
	return false;
}

bool TileMap::isStair(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x0, x1, y0, y1;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;

	// Recorremos el rango de tiles que ocupa el objeto (entre x0 a x1 y de y0 a y1)
	for (int y = y0; y <= y1; y++)
	{
		for (int x = x0; x <= x1; x++)
		{
			int tile = map[y * mapSize.x + x];  // Accedemos al tile en la posición (x, y)
			if (tile == 23)  // 23 es el identificador de la escalera
			{
				return true;
			}
		}
	}

	return false; 
}

bool TileMap::isLake(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x0, x1, y0, y1;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;

	// Recorremos el rango de tiles que ocupa el objeto (entre x0 a x1 y de y0 a y1)
	for (int y = y0; y <= y1; y++)
	{
		for (int x = x0; x <= x1; x++)
		{
			int tile = map[y * mapSize.x + x];  // Accedemos al tile en la posición (x, y)
			if (tile == 41 || tile == 24)  
			{
				return true;
			}
		}
	}

	return false;
}

bool TileMap::isDoor(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x0, x1, y0, y1;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;

	// Recorremos el rango de tiles que ocupa el objeto (entre x0 a x1 y de y0 a y1)
	for (int y = y0; y <= y1; y++)
	{
		for (int x = x0; x <= x1; x++)
		{
			int tile = map[y * mapSize.x + x];  // Accedemos al tile en la posición (x, y)
			if (tile == 20 || tile == 21 || tile == 22 || tile == 59)  
			{
				return true;
			}
		}
	}

	return false;
}


void TileMap::deleteRock(const glm::vec2 &pos) {
	for (int i = 0; i < objects.size(); ++i) {
		if (objects[i].position == pos) {
			objects.erase(objects.begin() + i);
			break;
		}
	}
}

void TileMap::addRock(const glm::vec2 &pos, int type) {
	objectInfo obj;
	obj.position = pos;
	obj.type = type;
	objects.push_back(obj);
}

std::vector<objectInfo>& TileMap::getObjects() {
	return objects;
}

float TileMap::distanceToGround(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;

	// Anem verificat cada tile per sota fins a trobar un bloc de col·lisió
	for (int j = y + 1; j < mapSize.y; ++j)
	{
		for (int x = x0; x <= x1; ++x)
		{
			int tile = map[j * mapSize.x + x];
			if (esColisio(tile))
			{
				// Retorna la distància entre l'enemic i el bloc de col·lisió
				std::cout << (j * tileSize) - (pos.y + size.y) << std::endl;
				return (j * tileSize) - (pos.y + size.y);
			}
		}
	}

	// Si no hi ha col·lisió a sota, retornem una gran distància
	return 10000.f;
}

