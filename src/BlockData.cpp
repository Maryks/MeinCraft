#include "BlockData.h"

Block::BlockData::BlockData()
{
	auto d1 = std::make_unique<Data>();
	m_DataHolder.insert(std::make_pair(Block::ID::Grass, std::move(d1)));
	m_DataHolder.at(Block::ID::Grass)->id = Block::ID::Grass;
	m_DataHolder.at(Block::ID::Grass)->topCoords.u = 0.f;
	m_DataHolder.at(Block::ID::Grass)->topCoords.v = 0.f;
	m_DataHolder.at(Block::ID::Grass)->sideCoords.u = 3.f;
	m_DataHolder.at(Block::ID::Grass)->sideCoords.v = 0.f;
	m_DataHolder.at(Block::ID::Grass)->bottomCoords.u = 2.f;
	m_DataHolder.at(Block::ID::Grass)->bottomCoords.v = 0.f;

	auto d2 = std::make_unique<Data>();
	m_DataHolder.insert(std::make_pair(Block::ID::Stone, std::move(d2)));
	m_DataHolder.at(Block::ID::Stone)->id = Block::ID::Stone;
	m_DataHolder.at(Block::ID::Stone)->topCoords.u = 1;
	m_DataHolder.at(Block::ID::Stone)->topCoords.v = 0;
	m_DataHolder.at(Block::ID::Stone)->sideCoords.u = 1;
	m_DataHolder.at(Block::ID::Stone)->sideCoords.v = 0;
	m_DataHolder.at(Block::ID::Stone)->bottomCoords.u = 1;
	m_DataHolder.at(Block::ID::Stone)->bottomCoords.v = 0;

	auto d3 = std::make_unique<Data>();
	m_DataHolder.insert(std::make_pair(Block::ID::Dirt, std::move(d3)));
	m_DataHolder.at(Block::ID::Dirt)->id = Block::ID::Dirt;
	m_DataHolder.at(Block::ID::Dirt)->topCoords.u = 2;
	m_DataHolder.at(Block::ID::Dirt)->topCoords.v = 0;
	m_DataHolder.at(Block::ID::Dirt)->sideCoords.u = 2;
	m_DataHolder.at(Block::ID::Dirt)->sideCoords.v = 0;
	m_DataHolder.at(Block::ID::Dirt)->bottomCoords.u = 2;
	m_DataHolder.at(Block::ID::Dirt)->bottomCoords.v = 0;

	m_stride = 16.f / 256.f;

	std::cout << "BlockData created." << std::endl;
}