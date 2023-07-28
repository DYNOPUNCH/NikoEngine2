#ifndef NIKO_TILEMAP_H
#define NIKO_TILEMAP_H

#include <vector>
#include <sstream>

#include "nikoCamera.h"
#include "nikoCollision.h"
#include "nikoSSU.h"

#include "include/pugi/pugiconfig.hpp"
#include "include/pugi/pugixml.hpp"

#include "player.h"

enum object_type
{
	FLOOR = 0,
	TALK = 1,
	DOOR = 2
};

struct NIKO_Map
{
	SDL_Rect CameraTarget;

	NIKO_Camera mapCamera;

	NIKO_Collision mcolli;

	struct mapinfo
	{
		std::string version;
		std::string tiledMapVersion;
		std::string orientation;
		std::string renderOrder;
		unsigned int width;
		unsigned int height;
		unsigned int tileWidth;
		unsigned int tileHeight;
		bool infinite;
		int nextLayerID;
		int nextObjectID;
	};

	mapinfo mapData;

	struct image
	{
		NIKO_Texture texture;
		std::string source;
		unsigned int width;
		unsigned int height;
	};

	struct tileset
	{
		int firstgid;
		std::string name;
		unsigned int tileWidth;
		unsigned int tileHeight;
		unsigned int tileCount;
		unsigned int columns;
		image tileImageData;
	};

	std::vector<tileset> tilesetData;

	struct gidTiles
	{
		std::string encoding;
		std::vector<int> gid;
	};

	struct layer
	{
		std::string id;
		std::string name;
		unsigned int width;
		unsigned int height;
		gidTiles gidData;
	};

	std::vector<layer> tileLayerData;

	struct imageLayer
	{
		std::string id;
		std::string name;
		int offsetx;
		int offsety;
		image imageSource;
	};

	std::vector<imageLayer> imageLayerData;

	struct objectProperty
	{
		std::string name;
		std::string value;
	};

	struct object
	{
		std::string id;
		object_type type;
		bool visible;
		int x;
		int y;
		int width;
		int height;
		std::vector<objectProperty> objectPropertyData;
	};

	struct objectGroup
	{
		std::string id;
		std::string name;
		std::vector<object> objectData;
	};

	std::vector<objectGroup> objectGroupData;

	std::string mappath;

	unsigned int tilelayercount;
	unsigned int objectlayercount;
	unsigned int imagelayercount;
	
	NIKO_Math logic;

	NIKO_Collision map_colli;

	unsigned int totalLayers;

	NIKO_Player *currentplayer;

	int textureFinder(int gidref);

	bool nikoParseTMX(const char* filename);

	void nikoClearTMX();

	void nikoRenderTileLayerTMX(unsigned int layer, int map_offsetx, int map_offsety);

	void nikoRenderImageLayerTMX(int layer, int map_offsetx, int map_offsety);

	void nikoCreateObjects(int groupnumber, int map_offsetx, int map_offsety, bool debugmode);

	void setPlayerPtr(NIKO_Player *playerptr)
	{
		currentplayer = playerptr;
	}

	NIKO_Map()
	{
		mapData.version = "";
		mapData.tiledMapVersion = "";
		mapData.orientation = "";
		mapData.renderOrder = "";
		mapData.width = 0;
		mapData.height = 0;
		mapData.tileWidth = 0;
		mapData.tileHeight = 0;
		mapData.infinite = false;
		mapData.nextLayerID = 0;
		mapData.nextObjectID = 0;

		tilesetData.clear();
		imageLayerData.clear();
		objectGroupData.clear();
		tilelayercount = 0;
		objectlayercount = 0;
		imagelayercount = 0;
		mappath = "";
	}

	~NIKO_Map()
	{
		//Clears all data from the TMX file. 

		mapData.version = "";
		mapData.tiledMapVersion = "";
		mapData.orientation = "";
		mapData.renderOrder = "";
		mapData.width = 0;
		mapData.height = 0;
		mapData.tileWidth = 0;
		mapData.tileHeight = 0;
		mapData.infinite = false;
		mapData.nextLayerID = 0;
		mapData.nextObjectID = 0;

		tilesetData.clear();
		imageLayerData.clear();
		objectGroupData.clear();
		tilelayercount = 0;
		objectlayercount = 0;
		imagelayercount = 0;

	}
};


#endif