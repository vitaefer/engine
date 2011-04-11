//arse.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <utility>
#include "Material.h"
#include "Entity.h"
#include "AseReader.h"

using namespace std;

string extract_string (istream &is, string s)
{
	int lastspace;
	getline(is,s);
	string b;
	lastspace=s.find_last_of(" ");
	b = s.substr(lastspace);
	return (b);
}

int extract_int (istream &is, string s)
{
	int lastspace;
	getline(is,s);
	int b;
	lastspace=s.find_last_of(" ");
	b = atoi(s.substr(lastspace).c_str());
	return (b);
}

unsigned int extract_singleint (string s, int pos)
{
	int len;
	len = (s.find(" ",pos))-(pos);
	unsigned int b;
	b=atoi(s.substr(pos,len).c_str());
	return (b);
}

float extract_vertexfloat (string s, int pos)
{
	int len;
	len = (s.find_first_of(" \t",pos))-(pos);
	float b;
	b=atof(s.substr(pos,len).c_str());
	return (b);
}

int extract_normalint(string s, int pos)
{
	int len;
	len = (s.find(" ",pos))-(pos);
	int b;
	b=atoi(s.substr(pos,len).c_str());
	return (b);
}

void extract_map_diffuse(istream &is, string s, Material &mat)
{
    int firstpos, lastpos;
    bool loop=true;
    while(loop)
    {
        getline(is,s);
        if(s.find("*BITMAP ") != string::npos)
        {
            firstpos=s.find("\"")+1;
            lastpos=s.find("\"",firstpos);
            LoadMaterial(s.substr(firstpos,lastpos-firstpos),mat);
            cout<<endl<<"Laddar: \""<<s.substr(firstpos,lastpos-firstpos)<<"\""<<endl;
        }
        else if(s.find("}") != string::npos)
        {
            loop=false;
        }
    }
}

void extract_material (istream &is, Material& mat)
{
    int pos;
    string s;
    bool loop=true;
    while(loop)
    {
        getline(is,s);
        if(s.find("*MATERIAL_AMBIENT") != string::npos)
        {
            pos=s.find("NT ")+3;
            mat.ambient[0] = extract_vertexfloat (s, pos);

            pos=s.find_first_not_of(" \t",pos);
            mat.ambient[1] = extract_vertexfloat (s, pos);

            pos=s.find_first_not_of(" \t",pos);
            mat.ambient[2] = extract_vertexfloat (s, pos);

            printf("\n ambient: %.2f %.2f %.2f \n",mat.ambient[0],mat.ambient[1],mat.ambient[2]);
        }
        else if(s.find("*MATERIAL_DIFFUSE") != string::npos)
        {
            pos=s.find("SE ")+3;
            mat.diffuse[0] = extract_vertexfloat (s, pos);

            pos=s.find_first_not_of(" \t",pos);
            mat.diffuse[1] = extract_vertexfloat (s, pos);

            pos=s.find_first_not_of(" \t",pos);
            mat.diffuse[2] = extract_vertexfloat (s, pos);
            printf("\n diffuse: %.2f %.2f %.2f \n",mat.diffuse[0],mat.diffuse[1],mat.diffuse[2]);
        }
        else if(s.find("*MATERIAL_SPECULAR") != string::npos)
        {
            pos=s.find("AR ")+3;
            pos=s.find_first_not_of(" \t",pos);
            mat.specular[0] = extract_vertexfloat (s, pos);

            pos=s.find_first_not_of(" \t",pos);
            mat.specular[1] = extract_vertexfloat (s, pos);

            pos=s.find_first_not_of(" \t",pos);
            mat.specular[2] = extract_vertexfloat (s, pos);
            printf("\n specular: %.2f %.2f %.2f \n",mat.specular[0],mat.specular[1],mat.specular[2]);
        }
        else if(s.find("*MATERIAL_SHINE ") != string::npos)
        {
            pos=s.find("NE ")+3;
            mat.shininess[0] = extract_vertexfloat (s, pos);
            printf("\n shininess: %.2f \n",mat.shininess[0]);
        }
        else if(s.find("*MAP_DIFFUSE {") != string::npos)
        {
            extract_map_diffuse(is,s,mat);
        }
        else if(s.find("}") != string::npos)
        {
            loop=false;
        }
    }
}

void extract_vertex_list(istream &is, vector<TVertex> &vectex)
{
    string s;
    bool loop = true;
    int x=0;
    int pos;
    TVertex tempvertex;
    while(loop == true)
    {
        getline(is,s);
        if (s.find("}") != string::npos) //escape da lup
        {
            loop = false;;
        }
        else
        {
            pos=s.find("X");
            pos=s.find_first_not_of(" \t",pos+1);

            //get x pos
            pos = (s.find_first_of(" \t",pos))+1;
            //MESH_VERTEX_LIST_X.push_back (extract_vertexfloat(s,pos));
            tempvertex.x = extract_vertexfloat(s,pos);
            //cout << tempvertex.x;
            //get y pos
            pos = (s.find_first_of(" \t",pos))+1;
            //MESH_VERTEX_LIST_Y.push_back (extract_vertexfloat(s,pos));
            tempvertex.y = extract_vertexfloat(s,pos);

            //get z pos
            pos = (s.find_first_of(" \t",pos))+1;
            //MESH_VERTEX_LIST_Z.push_back (extract_vertexfloat(s,pos));
            tempvertex.z = extract_vertexfloat(s,pos);
            tempvertex.val = x;
            tempvertex.sg = 0;

            vectex.push_back(tempvertex);

            x++;
        }
    }
}

void extract_face_list(istream &is,vector<TVertex> &vectex, vector<Face> &faceex)
{
    string s;
    bool loop=true;
    int x=0;
    int pos;
    Face tempface;
    unsigned int sg;
    TVertex tempvertex;
    while (loop == true)
    {
        getline(is,s);
        if (s.find("}") != string::npos) //escape da lup
        {
            loop = false;
        }
        else
        {
            pos=s.find("C")+1;
            //find n:
            pos=s.find_first_not_of(" \t",pos+1);
            //find blank, then A:, then blank
            pos=s.find_first_of(" \t",pos);
            pos=s.find_first_not_of(" \t",pos);
            pos=s.find_first_of(" \t",pos)+1;
            tempface.point[0] = extract_singleint(s,pos);

            pos=s.find_first_of(" \t",pos);
            pos=s.find_first_not_of(" \t",pos);
            pos=s.find_first_of(" \t",pos)+1;
            tempface.point[1] = extract_singleint(s,pos);

            pos=s.find_first_of(" \t",pos);
            pos=s.find_first_not_of(" \t",pos);
            pos=s.find_first_of(" \t",pos)+1;
            tempface.point[2] = extract_singleint(s,pos);

            pos=s.find("G");
            pos=s.find_first_not_of(" \t",pos+1);
            sg = extract_singleint(s,pos);

            for(unsigned int i=0; i < vectex.size(); i++)
            {
                if (vectex.at(i).sg != sg && vectex.at(i).val == tempface.point[0])
                {
                    if (vectex.at(i).sg == 0)
                    {
                        vectex.at(i).sg = sg;
                    }
                    else
                    {
                        tempvertex.x = vectex.at(i).x;
                        tempvertex.y = vectex.at(i).y;
                        tempvertex.z = vectex.at(i).z;
                        tempvertex.val = vectex.at(i).val;
                        tempvertex.sg = sg;
                        vectex.push_back(tempvertex);
                        tempface.point[0] = vectex.size()-1;
                    }
                }
                if (vectex.at(i).sg != sg && vectex.at(i).val == tempface.point[1])
                {
                    if (vectex.at(i).sg == 0)
                    {
                        vectex.at(i).sg = sg;
                    }
                    else
                    {
                        tempvertex.x = vectex.at(i).x;
                        tempvertex.y = vectex.at(i).y;
                        tempvertex.z = vectex.at(i).z;
                        tempvertex.val = vectex.at(i).val;
                        tempvertex.sg = sg;
                        vectex.push_back(tempvertex);
                        tempface.point[1] = vectex.size()-1;
                    }
                }
                if (vectex.at(i).sg != sg && vectex.at(i).val == tempface.point[2])
                {
                    if (vectex.at(i).sg == 0)
                    {
                        vectex.at(i).sg = sg;
                    }
                    else
                    {
                        tempvertex.x = vectex.at(i).x;
                        tempvertex.y = vectex.at(i).y;
                        tempvertex.z = vectex.at(i).z;
                        tempvertex.val = vectex.at(i).val;
                        tempvertex.sg = sg;
                        vectex.push_back(tempvertex);
                        tempface.point[2] = vectex.size()-1;
                    }
                }
            }

            faceex.push_back(tempface);

            x++;
        }
    }
}

void extract_tvertex_list(istream& is, vector<float>& MESH_TVERTLIST_U, vector<float>& MESH_TVERTLIST_V)
{
    string s;
    bool loop=true;
    int x=0;
    int pos;
    while (loop == true)
    {
        getline(is,s);
        if (s.find("}") != string::npos) //escape da lup
        {
            loop = false;
        }
        else
        {
            pos=s.find("R")+1;
            pos=s.find_first_not_of(" \t",pos+1);
            pos = (s.find_first_of(" \t",pos))+1;

            //cout<<"hej "<<s<<endl;
            //cout<<"pos "<<extract_vertexfloat(s,pos)<<endl;

            MESH_TVERTLIST_U.push_back(extract_vertexfloat(s,pos));
            //cout<<"value "<<MESH_TVERTLIST_U.at(MESH_TVERTLIST_U.size()-1)<<endl;
            pos = (s.find_first_of(" \t",pos))+1;
            MESH_TVERTLIST_V.push_back(extract_vertexfloat(s,pos));
        }
        x++;
    }
}

void extract_tface_list(istream& is, vector<TVertex>& vectex, vector<Face>& faceex, vector<float>& MESH_TVERTLIST_U, vector<float>& MESH_TVERTLIST_V)
{
    cout << vectex.size() << endl;
    string s;
    bool loop=true;
    int x=0;
    int pos;
    int p0, p1, p2, index;
    while (loop == true)
    {
        getline(is,s);
        if (s.find("}") != string::npos) //escape da lup
        {
            loop = false;;
        }
        else
        {
            pos=s.find("C")+1;
            pos=s.find_first_of(" \t",pos)+1;
            index = extract_singleint(s,pos);

            cout<<"index "<<index<<endl;
            //get first point
            pos = (s.find_first_of(" \t",pos))+1;
            p0 = extract_singleint(s,pos);
            //get second point
            pos = (s.find_first_of(" \t",pos))+1;
            p1 = extract_singleint(s,pos);
            //get third point
            pos = (s.find_first_of(" \t",pos))+1;
            p2 = extract_singleint(s,pos);

            vectex.at( faceex.at(index).point[0] ).u = MESH_TVERTLIST_U.at(p0);
            vectex.at( faceex.at(index).point[0] ).v = MESH_TVERTLIST_V.at(p0);

            vectex.at( faceex.at(index).point[1] ).u = MESH_TVERTLIST_U.at(p1);
            vectex.at( faceex.at(index).point[1] ).v = MESH_TVERTLIST_V.at(p1);

            vectex.at( faceex.at(index).point[2] ).u = MESH_TVERTLIST_U.at(p2);
            vectex.at( faceex.at(index).point[2] ).v = MESH_TVERTLIST_V.at(p2);
        }
        x++;
    }
}

void extract_mesh(istream& is, Mesh& mesh)
{
	vector<float>	MESH_VERTEX_LIST_X, MESH_VERTEX_LIST_Y, MESH_VERTEX_LIST_Z,
								MESH_TVERTLIST_U, MESH_TVERTLIST_V;
    vector<int> MESH_VERTEX_SMOOTHING, MESH_VERTEX_TIMESLOADED;

    vector<TVertex> vectex;
	vector<Face> faceex;

    string s;
    bool loop=true;

    while(loop)
    {
        getline(is,s);
        if (s.find("*MESH_VERTEX_LIST {") != string::npos)
        {
            extract_vertex_list(is, vectex);
        }
        else if (s.find("*MESH_FACE_LIST {") != string::npos)
        {
            extract_face_list(is, vectex, faceex);
        }
        else if (s.find("*MESH_TVERTLIST {") != string::npos)
        {
            extract_tvertex_list(is,MESH_TVERTLIST_U,MESH_TVERTLIST_V);
        }
        else if (s.find("*MESH_TFACELIST {") != string::npos)
        {
            extract_tface_list(is,vectex,faceex,MESH_TVERTLIST_U,MESH_TVERTLIST_V);
        }
        else if (s.find("}") != string::npos)
        {
            loop=false;
        }
    }
	cout<<endl<<endl<<"DONE!"<<endl;
	mesh.init(vectex.size(),faceex.size());
    unsigned int i;

    cout<<endl<<"Vertices "<<vectex.size()<<endl<<endl;

    for(i=0;i<vectex.size();i++)
    {
        mesh.vertex[i].x=vectex.at(i).x;
        mesh.vertex[i].y=vectex.at(i).y;
        mesh.vertex[i].z=vectex.at(i).z;

        mesh.vertex[i].u=vectex.at(i).u;
        mesh.vertex[i].v=vectex.at(i).v;

        printf("vertex[%i] x:%0.1f y:%0.1f z:%0.1f   u:%0.1f v:%0.1f  sg:%i \n",i,mesh.vertex[i].x,mesh.vertex[i].y,mesh.vertex[i].z,mesh.vertex[i].u,mesh.vertex[i].v, vectex.at(i).sg);
    }

    cout<<endl<<"Faces "<<faceex.size()<<endl<<endl;

    for(i=0;i<faceex.size();i++)
    {
        mesh.face[i].point[0]=faceex.at(i).point[0];
        mesh.face[i].point[1]=faceex.at(i).point[1];
        mesh.face[i].point[2]=faceex.at(i).point[2];

        printf("face[%i] p0:%i p1:%i p2:%i \n",i,mesh.face[i].point[1],mesh.face[i].point[0],mesh.face[i].point[2]);
    }

    mesh.calculateNormals();
    mesh.createBuffers();
}

int LoadAse(const string &filename, StaticEntity &entity)
{
	//constants
	string SCENE_FILENAME;
	int SCENE_FIRSTFRAME, SCENE_LASTFRAME, TIMEVALUE, MATERIAL_COUNT;

    vector<string> ENTITY_NAME;
    vector<Material*> MATERIAL_LIST;
    vector<Mesh*> MESH_LIST;
    vector<int> MATERIAL_INDEX;

    Material *matptr;
    Mesh *meshptr;

	//variables
    filebuf fb;
    fb.open (filename.c_str(),ios::in);
    istream is(&fb);
	string s;
	int pos;

	while (getline(is, s))
	{
		if (s.find("*SCENE {") != string::npos)
		{
			SCENE_FILENAME = extract_string(is,s);
			//cout << "SCENE_FILENAME " << SCENE_FILENAME <<  endl;

			SCENE_FIRSTFRAME = extract_int(is,s);
			//cout << "SCENE_FIRSTFRAME " << SCENE_FIRSTFRAME <<  endl;

			SCENE_LASTFRAME = extract_int(is,s);
			//cout << "SCENE_LASTFRAME " << SCENE_LASTFRAME <<  endl;

			//omitting the following constants for now
			getline(is, s); //SCENE_FRAMESPEED
			getline(is, s); //SCENE_TICKSPERFRAME
			getline(is, s); //SCENE_BACKGROUND_STATIC
			getline(is, s); //SCENE_AMBIENT_STATIC
		}
		else if (s.find("*MATERIAL_LIST {") != string::npos)
		{
		    MATERIAL_COUNT = extract_int(is,s);
		    bool loop = true;
			while(loop == true)
			{
				getline(is,s);
				if (s.find("}") != string::npos) //escape da lup
				{
					loop = false;;
				}
				else
				{
				    cout<<endl<<"NEW MAT "<<endl;
				    matptr = new Material();
                    extract_material(is,*matptr);
                    MATERIAL_LIST.push_back(matptr);
				}
			}
		}
		else if (s.find("*GEOMOBJECT {") != string::npos)
		{
			ENTITY_NAME.push_back(extract_string(is,s));
			//cout << "NODE_NAME" << NODE_NAME <<  endl;
			//omitting the following constants for now
			bool loop=true;
			while(loop)
			{
			    getline(is,s);
			    if (s.find("}") != string::npos)
                    loop=false;
			}

			loop=true;
			while(loop)
			{
			    getline(is,s);
			    if (s.find("*MESH {") != string::npos)
			    {
			        cout<<endl<<"NEW MESH "<<endl;
			        meshptr = new Mesh();
                    extract_mesh(is,*meshptr);
                    MESH_LIST.push_back(meshptr);
                    loop=false;
			    }
			}
			getline(is,s);
			getline(is,s);
			getline(is,s);
            MATERIAL_INDEX.push_back(extract_int(is,s));
		}
    }


 //   Om det finns flera meshes samma ase-fil s� l�gg alla dessa som children i entity
    StaticEntity *entityptr;

    if(MESH_LIST.size()>1)
    {
        for(unsigned int i=0; i<MESH_LIST.size(); i++)
        {
            entityptr = new StaticEntity();
            entityptr->SetName(ENTITY_NAME.at(i));
            entityptr->mesh = MESH_LIST.at(i);
            entityptr->material = MATERIAL_LIST.at(MATERIAL_INDEX.at(i));
            entity.AddChild(*entityptr);
        }
    }
 //   Annars s� l�ggs mesh och material direkt i entity
    else
    {
        entity.SetName(ENTITY_NAME.at(0));
        entity.mesh = MESH_LIST.at(0);
        entity.material = MATERIAL_LIST.at(MATERIAL_INDEX.at(0));
    }

	return 1; //success!
}

StaticEntity &LoadAse(const string &filename)
{
    StaticEntity *entity = new StaticEntity();
    LoadAse(filename, *entity);
    return *entity;
}
