#include <iostream>
#include <QFile>
#include <QRegularExpression>
#include <QTextStream>

#include "objreader.h"
#include "pathconfig.h"
#include "ltr_gui_prefs.h"

int cnt = 0;
int vcnt = 0;
int tcnt = 0;

object_t object;
std::vector<object_t> object_table;

static void add_vertex(float x, float y, float z, 
                float nx, float ny, float nz,
                float s, float t)
{
  ++cnt;
  object.vtx_table.push_back((vtx_t){x, y, z, nx, ny, nz, s, t});
}

static void add_index(int index)
{
  ++vcnt;
  object.vtx_indices.push_back(index);
}

static void add_tris(int offset, int count, bool glass)
{
  ++tcnt;
  object.tris_table.push_back((tri_t){offset, count, glass});
}

bool glass = false;
static QRegularExpression vt_line(QString::fromUtf8("^\\s*VT\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s*$"));
static QRegularExpression idx10_line(QString::fromUtf8("^\\s*IDX10\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s*$"));
static QRegularExpression idx_line(QString::fromUtf8("^\\s*IDX\\s+(\\S+)\\s*$"));
static QRegularExpression tris_line(QString::fromUtf8("^\\s*TRIS\\s+(\\S+)\\s+(\\S+)\\s*$"));
static QRegularExpression texture_line(QString::fromUtf8("^\\s*TEXTURE\\s+(.*)\\s*$"));
static QRegularExpression glass_line(QString::fromUtf8("^\\s*GLASS\\s*$"));


static void process_line(const QString &line)
{
  QRegularExpressionMatch match;
  match = vt_line.match(line);
  if(match.hasMatch()){
    float x, y, z, nx, ny, nz, s, t;
    x = match.captured(1).toFloat(); 
    y = match.captured(2).toFloat(); 
    z = match.captured(3).toFloat(); 
    nx = match.captured(4).toFloat(); 
    ny = match.captured(5).toFloat(); 
    nz = match.captured(6).toFloat(); 
    s = match.captured(7).toFloat(); 
    t = match.captured(8).toFloat(); 
    add_vertex(x, y, z, nx, ny, nz, s, t);
  }else{
    match = idx10_line.match(line);
    if(match.hasMatch()){
      add_index(match.captured(1).toInt());
      add_index(match.captured(2).toInt());
      add_index(match.captured(3).toInt());
      add_index(match.captured(4).toInt());
      add_index(match.captured(5).toInt());
      add_index(match.captured(6).toInt());
      add_index(match.captured(7).toInt());
      add_index(match.captured(8).toInt());
      add_index(match.captured(9).toInt());
      add_index(match.captured(10).toInt());
    }else{
      match = idx_line.match(line);
      if(match.hasMatch()){
        add_index(match.captured(1).toInt());
      }else{
        match = tris_line.match(line);
        if(match.hasMatch()){
          add_tris(match.captured(1).toInt(), match.captured(2).toInt(), glass);
          glass = false;
        }else{
          match = texture_line.match(line);
          if(match.hasMatch()){
            if(!match.captured(1).isEmpty()){
              object.texture = PrefProxy::getDataPath(match.captured(1));
              //std::cout<<"Texture: "<<qPrintable(object.texture)<<"\n";
            }
          }else{
            match = glass_line.match(line);
            if(match.hasMatch()){
              glass = true;
            }
          }
        }
      }
    }
  }
}

static void obj_init(object_t &obj)
{
  obj.vtx_table.clear();
  obj.vtx_indices.clear();
  obj.tris_table.clear();
  obj.texture = QString::fromUtf8("");
}

void read_obj()
{
  char *obj_list[] = {(char *)"sphere.obj", (char *)"sparow_opaq.obj", 
                      (char *)"sparow_glass.obj", NULL};
  
  for(int i = 0; obj_list[i] != NULL; ++i){
    QFile f(PrefProxy::getDataPath(QString::fromUtf8(obj_list[i])));
    obj_init(object); 
    cnt = 0;
    vcnt = 0;
    tcnt = 0;

    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
      continue;

    QTextStream in(&f);
    while(!in.atEnd()){
      QString line = in.readLine();
      process_line(line);
    }
    f.close();
    object_table.push_back(object);
    //std::cout<<(char *)obj_list[i]<<"\n";
    //std::cout<< cnt <<" vertices processed!\n";
    //std::cout<< vcnt <<" indexes processed!\n";
    //std::cout<< tcnt <<" tris processed!\n";
  }
  
}
