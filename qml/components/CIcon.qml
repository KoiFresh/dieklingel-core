import QtQml 2.15
import QtQuick 2.12
import QtQuick.Controls 2.12

Text {
	property string icon: ""

	FontLoader {
		source: "../fonts/MaterialSymbolsOutlined[FILL=0,GRAD=25,opsz=24,wght=100].ttf"
	}

	text: icon
	font {
		family: "MaterialSymbolsOutlined"
	}
}
