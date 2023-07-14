#include <flatbuffers/flatbuffers.h>

#include <fstream>
#include <iostream>

#include "monster_generated.h"  // 包含生成的 Flatbuffers 代码

using namespace std;
using namespace MyGame::Example;  // 使用命名空间

int main() {
  // 创建一个 Flatbuffers 构造器，用于构造序列化后的数据
  flatbuffers::FlatBufferBuilder builder;

  // 创建武器 和 盾牌
  auto weapon1 = CreateWeapon(builder, builder.CreateString("Sword"), 50);
  auto weapon2 = CreateWeapon(builder, builder.CreateString("Axe"), 75);
  vector<flatbuffers::Offset<Weapon>> weapons_vector;
  weapons_vector.push_back(weapon1);
  weapons_vector.push_back(weapon2);
  auto weapons = builder.CreateVector(weapons_vector);
  auto shield = CreateShield(builder, builder.CreateString("Iron Shield"), 100);

  // monster 添加内容
  auto monster = CreateMonster(
      builder, CreateVec3(builder, 1.0f, 2.0f, 3.0f), 300,
      builder.CreateString("Orc"),
      builder.CreateVector(vector<uint8_t>{1, 2, 3, 4, 5}), Color::Color_RED,
      weapons, Equipment::Equipment_Shield, shield.Union());

  // 将 Monster 对象写入 Flatbuffers 缓冲区
  builder.Finish(monster);

  // 将 Flatbuffers 缓冲区写入磁盘文件
  ofstream outfile("data/monster.bin", ios::binary);
  outfile.write((const char*)builder.GetBufferPointer(), builder.GetSize());
  outfile.close();

  // 从磁盘文件读取 Flatbuffers 缓冲区
  ifstream infile("data/monster.bin", ios::binary | ios::ate);
  streamsize size = infile.tellg();
  infile.seekg(0, ios::beg);
  vector<char> buffer(size);
  if (infile.read(buffer.data(), size)) {
    // 从 Flatbuffers 缓冲区中读取 Monster 对象

    auto monster2 = GetMonster(buffer.data());

    // 输出 Monster 对象的属性
    cout << "Monster name: " << monster2->name()->str() << endl;
    cout << "Monster HP: " << monster2->hp() << endl;
    cout << "Monster position: (" << monster2->pos()->x() << ", "
         << monster2->pos()->y() << ", " << monster2->pos()->z() << ")" << endl;
    cout << "Monster color: " << EnumNameColor(monster2->color()) << endl;
    cout << "Monster inventory: ";
    for (int i = 0; i < monster2->inventory()->size(); i++) {
      cout << (int)monster2->inventory()->Get(i) << " ";
    }
    cout << endl;
    cout << "Monster weapons: ";
    for (int i = 0; i < monster2->weapons()->size(); i++) {
      auto weapon = monster2->weapons()->Get(i);
      cout << "Name: " << weapon->name()->str() << ", ";
      cout << "Damage: " << weapon->damage() << "; ";
    }
    cout << endl;
    auto equipped = monster2->equipped_type();
    if (equipped == Equipment::Equipment_Shield) {
      auto shield = monster2->equipped_as_Shield();
      cout << "Monster shield: " << shield->name()->str() << ", ";
      cout << "Defense: " << shield->defense() << endl;
    }
  }

  return 0;
}