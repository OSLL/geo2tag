#include "GetGoogleMarks.h"
#include "Position.h"



GetGoogleMarks::GetGoogleMarks(QString map_url,Position user_pos,double radius,QStringList  cookies)
{
m_map_url=map_url;
m_user_pos=user_pos;
m_radius=radius;
m_cookies=cookies;
client=new QHttp("maps.google.com");
execute();
}

void GetGoogleMarks::getXml(){
    QDomDocument xml;
    QString labelCoords=m_user_pos.toStr().c_str();//Добавляем высоту над уровнем моря
    QDomNodeList  names,descriptions,coordinates;//Список тегов координат
    QDomNode node;
    int count;
    xml.setContent(client->readAll());//Помещаем содержимое файла map.xml в объект xml
    coordinates=xml.elementsByTagName("coordinates");//Выбираем все элементы с тегом coordinates
    names=xml.elementsByTagName("name");//Все элементы с тегом name
    descriptions=xml.elementsByTagName("description");//Все элементы с тегом description
    count=coordinates.count();
    m_marks.clear();
    for (int i=0;i<count;i++){
        node=coordinates.item(i);
            if (Position(node.firstChild().toText().data().toStdString().c_str()).distance(m_user_pos)<=m_radius){
                m_marks.push_back(descriptions.item(i+1).firstChild());
                //Если метка лежит в радиусе заданом ползователем, от его местоположения
                //В двух вышеназваных списках первые элементы это имя и описание карты
               /* str+="Name: "+names.item(i+1).firstChild().toText().data()+
                   +"\nCoordinates: "+labelCoords
                   +"\nDescriptions: "+descriptions.item(i+1).firstChild().toText().data()+"\n";*/
            }
        }
    emit done();
}

void GetGoogleMarks::execute(){
    QHttpRequestHeader header("GET",this->m_map_url+ "&output=kml");//Заголовок запроса
    QString cookieStr;
    header.setValue("Host","maps.google.com");//Заполняем поле host
    QStringList cookies=getCookies();
    for (QStringList::iterator i=cookies.begin();i!=cookies.end();i++) cookieStr+=*i+"; ";//Добавляем cookie, полученные при авторизации
    header.setValue("Cookie",cookieStr);//Заполняем поле cookie в заголовке запроса
    header.setContentType("application/x-www-form-urlencoded");
    client->request(header,0);//Посылка запроса

}

const QString& GetGoogleMarks::getMapUrl() const{
return m_map_url;
}

const QString& GetGoogleMarks::setMapUrl(const QString& mapUrl){
    m_map_url=mapUrl;
    return m_map_url;
}


const Position& GetGoogleMarks::getUserPos() const{
return m_user_pos;
}

const Position& GetGoogleMarks::setUserPos(const Position& pos){
m_user_pos=pos;
return m_user_pos;
}

const double GetGoogleMarks::getRadius() const{
return m_radius;
}

const double GetGoogleMarks::setRadius(double radius){
m_radius=radius;
return m_radius;
}



const QStringList& GetGoogleMarks::getCookies()const{
return m_cookies;
}

const QStringList& GetGoogleMarks::setCookies(const QStringList& cookies){
m_cookies=cookies;
return m_cookies;
}

const QList<QDomNode>& GetGoogleMarks::getMarks() const{
return m_marks;
}
