#include "nikoTileMap.h"

//Add all object types here and in the object type array in tilemap.hpp.
std::string objarray[] =
{
	"FLOOR",
	"TALK",
	"DOOR"
};

object_type GetObjectType(std::string objectype_str)
{
	int inttype = FLOOR;

	for (int i = 0; i < sizeof(objarray) / sizeof(objarray[0]); i++)
		if (objectype_str == objarray[i])
			return static_cast<object_type>(inttype);
		else
			inttype++;

	return FLOOR;
}





bool NIKO_Map::nikoParseTMX(const char* filename)
{
	if (filename == mappath)
		return false;
	else
		mappath = filename;

	//Loads the document, if document does not load exit the fucntion.
	pugi::xml_document doc;
	if (!doc.load_file(filename))
		return false;

	tilelayercount = 0;
	objectlayercount = 0;
	imagelayercount = 0;

	//makes a node labeled map. This acts as a pointer to "map" in the xml file.
	pugi::xml_node maps = doc.child("map");

	//Stores the map width and height attributes into integers.
	for (pugi::xml_node map = maps; map; map = map.next_sibling("map"))
	{
		mapData.version = map.attribute("version").as_string();
		mapData.tiledMapVersion = map.attribute("tiledversion").as_string();
		mapData.orientation = map.attribute("orientation").as_string();
		mapData.renderOrder = map.attribute("renderorder").as_string();
		mapData.width = map.attribute("width").as_uint();
		mapData.height = map.attribute("height").as_uint();
		mapData.tileWidth = map.attribute("tilewidth").as_uint();
		mapData.tileHeight = map.attribute("tileheight").as_uint();
		mapData.infinite = map.attribute("infinite").as_bool();
		mapData.nextLayerID = map.attribute("nextlayerid").as_int();
		mapData.nextObjectID = map.attribute("nextobjectid").as_int();
	}

	//makes a node labeled tilesets. This acts as a pointer to "tileset" which is a child of "map" in the xml file.
	pugi::xml_node tilesets = doc.child("map").child("tileset");

	//Stores the base tile and tile width/height of the map into vectors using the "pushback" class object.
	for (pugi::xml_node tilesetx = tilesets; tilesetx; tilesetx = tilesetx.next_sibling("tileset"))
	{
		tileset tempTileset;

		tempTileset.firstgid = tilesetx.attribute("firstgid").as_int();
		tempTileset.name = tilesetx.attribute("name").as_int();
		tempTileset.tileWidth = tilesetx.attribute("tilewidth").as_int();
		tempTileset.tileHeight = tilesetx.attribute("tileheight").as_int();
		tempTileset.tileCount = tilesetx.attribute("tilecount").as_int();
		tempTileset.columns = tilesetx.attribute("columns").as_int();

		//Stores the texture source and the width and height of image.
		for (pugi::xml_node image = tilesetx.child("image"); image; image = image.next_sibling("image"))
		{
			std::string fullname = "";

			fullname += "resc/field/";
			fullname += image.attribute("source").as_string();

			tempTileset.tileImageData.texture.nikoTexLibraryAdd(image.attribute("source").as_string(), fullname.c_str());

			tempTileset.tileImageData.source = image.attribute("source").as_string();
			tempTileset.tileImageData.width = image.attribute("width").as_uint();
			tempTileset.tileImageData.height = image.attribute("width").as_uint();
		}

		tilesetData.push_back(tempTileset);

	}


	//makes a node labeled layers. This acts as a pointer to "layer" which is a child of "map" in the xml file.
	pugi::xml_node layers = doc.child("map").child("layer");

	//Stores the layer information into vectors. 
	for (pugi::xml_node layerx = layers; layerx; layerx = layerx.next_sibling("layer"))
	{
		layer templayer;

		templayer.id = layerx.attribute("id").as_string();
		templayer.name = layerx.attribute("name").as_string();
		templayer.width = layerx.attribute("width").as_uint();
		templayer.height = layerx.attribute("height").as_uint();


		//Stands for "Multilayer" gid. Stores the gid information of every tile into a vector. 
		for (pugi::xml_node gidx = layerx.child("data"); gidx; gidx = gidx.next_sibling("data"))
		{
			templayer.gidData.encoding = gidx.attribute("encoding").as_string();

			std::string tempstr = gidx.text().as_string();
			std::stringstream s_stream(tempstr);

			while (s_stream.good()) 
			{
				std::string substr;
				std::getline(s_stream, substr, ',');
				templayer.gidData.gid.push_back(std::stoi(substr));
			}

			tileLayerData.push_back(templayer);
			tilelayercount++;
		}

	}

	pugi::xml_node imagelayers = doc.child("map").child("imagelayer");

	for (pugi::xml_node imagelayerx = imagelayers; imagelayerx; imagelayerx = imagelayerx.next_sibling("imagelayer"))
	{
		imageLayer tempimagelayer;

		tempimagelayer.id = imagelayerx.attribute("id").as_string();
		tempimagelayer.name = imagelayerx.attribute("name").as_string();
		tempimagelayer.offsetx = imagelayerx.attribute("offsetx").as_int();
		tempimagelayer.offsety = imagelayerx.attribute("offsety").as_int();

		for (pugi::xml_node image = imagelayerx.child("image"); image; image = image.next_sibling("image"))
		{
			std::string fullname = "";

			fullname += "resc/field/";
			fullname += image.attribute("source").as_string();

			tempimagelayer.imageSource.texture.nikoTexLibraryAdd(image.attribute("source").as_string(), fullname.c_str());

			tempimagelayer.imageSource.source = image.attribute("source").as_string();
			tempimagelayer.imageSource.width = image.attribute("width").as_uint();
			tempimagelayer.imageSource.height = image.attribute("height").as_uint();
		}

		imageLayerData.push_back(tempimagelayer);
		imagelayercount++;
	}

	pugi::xml_node objectgroups = doc.child("map").child("objectgroup");

	for (pugi::xml_node objectgroupx = objectgroups; objectgroupx; objectgroupx = objectgroupx.next_sibling("objectgroup"))
	{
		objectGroup tempobjectgroup;

		tempobjectgroup.id = objectgroupx.attribute("id").as_string();
		tempobjectgroup.name = objectgroupx.attribute("name").as_string();

		//stores all genric objects such as enemies and collision boxes
		for (pugi::xml_node objectx = objectgroupx.child("object"); objectx; objectx = objectx.next_sibling("object"))
		{
			object tempobject;

			if (objectx.attribute("id"))
				tempobject.id = objectx.attribute("id").as_string();
			else
				tempobject.id = "";

			if (objectx.attribute("type"))
				tempobject.type = GetObjectType(objectx.attribute("type").as_string());
			else
				tempobject.type = FLOOR;

			if (objectx.attribute("visible"))
				tempobject.visible = objectx.attribute("visible").as_bool();
			else
				tempobject.visible = true;

			if (objectx.attribute("x"))
				tempobject.x = objectx.attribute("x").as_int();
			else
				tempobject.x = 0;

			if (objectx.attribute("y"))
				tempobject.y = objectx.attribute("y").as_int();
			else
				tempobject.y = 0;

			if (objectx.attribute("width"))
				tempobject.width = objectx.attribute("width").as_int();
			else
				tempobject.width = 0;

			if (objectx.attribute("height"))
				tempobject.height = objectx.attribute("height").as_int();
			else
				tempobject.height = 0;

			for (pugi::xml_node objectpropertyx = objectx.child("properties").child("property"); objectpropertyx; objectpropertyx = objectpropertyx.next_sibling("property"))
			{
				objectProperty tempobjectproperty;
				tempobjectproperty.name = objectpropertyx.attribute("name").as_int();
				tempobjectproperty.value = objectpropertyx.attribute("value").as_int();
				tempobject.objectPropertyData.push_back(tempobjectproperty);
			}

			objectlayercount++;
			tempobjectgroup.objectData.push_back(tempobject);
		}

		objectGroupData.push_back(tempobjectgroup);
	}

	totalLayers = tilelayercount + objectlayercount + imagelayercount;

	return true;
}

void NIKO_Map::nikoClearTMX()
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

int NIKO_Map::textureFinder(int gidref)
{
	int texCounter = 0;

	if (gidref == 0 || tilesetData.size() == 1)
		return 0;

	//Run logic tests till you get the right texture
	for (int i = 0; i < tilesetData.size(); i++)
	{
		//If the gid is in range then return texture number. If not, incriment it and run it back.
		if (i < tilesetData.size() - 1)
		{
			if (logic.nikoInRange(tilesetData[i].firstgid, tilesetData[i + 1].firstgid-1, gidref))
				return texCounter;
			else
				texCounter++;
		}
		else
			return texCounter;
	}

	return texCounter;
}

void NIKO_Map::nikoRenderTileLayerTMX(unsigned int layer, int map_offsetx, int map_offsety)
{
	int layerprotect = 0;

	if (layer < tilelayercount)
		layerprotect = layer;
	else
		layerprotect = 0;

	//if(currentplayer != NULL)
	//currentplayer->renderPlayer();
	
	for (unsigned int gidittr = 0, snx = 1, sny = 1, currentlayer = 0; gidittr < tileLayerData[layerprotect].gidData.gid.size(); gidittr++, snx++)
	{
		
		//Map math 
		if (snx > mapData.width)
		{
			sny++;
			snx = 1;
		}

		if (sny > mapData.height)
		{
			sny = 1;
			currentlayer++;
		} 

		if (tileLayerData[layerprotect].gidData.gid[gidittr] > 0)
		{
			int currentTexture = 0;

			currentTexture = textureFinder(tileLayerData[layerprotect].gidData.gid[gidittr]);
			
			int tile_id = tileLayerData[layerprotect].gidData.gid[gidittr] - tilesetData[currentTexture].firstgid;
			int set_width = tilesetData[currentTexture].tileImageData.width / tilesetData[currentTexture].tileWidth;

			int tile_x = tile_id % set_width;
			int tile_y = std::floor(tile_id / set_width);

			tilesetData[currentTexture].tileImageData.texture.nikoRenderTexture(
				tilesetData[currentTexture].tileImageData.source,
				tilesetData[currentTexture].tileWidth,
				tilesetData[currentTexture].tileHeight,
				tile_x * tilesetData[currentTexture].tileWidth,
				tile_y * tilesetData[currentTexture].tileHeight,
				((snx - 1) * tilesetData[currentTexture].tileWidth + map_offsetx),
				((sny - 1) * tilesetData[currentTexture].tileHeight + map_offsety),
				tilesetData[currentTexture].tileWidth,
				tilesetData[currentTexture].tileHeight,
				SDL_FLIP_NONE);
			
		}
	}
}

void NIKO_Map::nikoRenderImageLayerTMX(int layer, int map_offsetx, int map_offsety)
{
	int layerprotect = 0;

	if (layer < imagelayercount)
		layerprotect = layer;
	else
		layerprotect = 0;

	if(currentplayer != NULL)
	currentplayer->renderPlayer();

	for (int i = 0; i < imageLayerData.size(); i++)
	{
		imageLayerData[layerprotect].imageSource.texture.nikoRenderTexture(
			imageLayerData[layerprotect].imageSource.source,
			imageLayerData[layerprotect].imageSource.width,
			imageLayerData[layerprotect].imageSource.height,
			0,
			0,
			imageLayerData[layerprotect].offsetx + map_offsetx,
			imageLayerData[layerprotect].offsety + map_offsety,
			imageLayerData[layerprotect].imageSource.width,
			imageLayerData[layerprotect].imageSource.height,
			SDL_FLIP_NONE);
	}
}

void NIKO_Map::nikoCreateObjects(int groupnumber, int map_offsetx, int map_offsety, bool debugmode)
{
	for (int i = 0; i < objectGroupData[groupnumber].objectData.size(); i++)
	{
		if (objectGroupData[groupnumber].objectData[i].type == FLOOR)
		{
			//debug stuff
			if(debugmode == true)
			logic.nikoRectangle(
				objectGroupData[groupnumber].objectData[i].x + map_offsetx,
				objectGroupData[groupnumber].objectData[i].y + map_offsety,
				objectGroupData[groupnumber].objectData[i].width,
				objectGroupData[groupnumber].objectData[i].height,
				{ 77, 201, 255 },
				255,
				true);

			map_colli.nikoUnmoveObject(
				objectGroupData[groupnumber].objectData[i].x,
				objectGroupData[groupnumber].objectData[i].y,
				objectGroupData[groupnumber].objectData[i].width,
				objectGroupData[groupnumber].objectData[i].height,
				currentplayer->x,
				currentplayer->y,
				currentplayer->width,
				currentplayer->height,
				0, 
				0);
		}
		else if (objectGroupData[groupnumber].objectData[i].type == TALK)
		{
			//debug stuff
			if (debugmode == true)
				logic.nikoRectangle(
					objectGroupData[groupnumber].objectData[i].x + map_offsetx,
					objectGroupData[groupnumber].objectData[i].y + map_offsety,
					objectGroupData[groupnumber].objectData[i].width,
					objectGroupData[groupnumber].objectData[i].height,
					{ 77, 255, 193 },
					255,
					true);
		}
	}
}

