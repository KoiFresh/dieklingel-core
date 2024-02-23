import QtQml 2.15
import QtQuick 2.12
import QtQuick.Controls 2.12



Text {
	property string icon: ""

	FontLoader {
		source: "../fonts/MaterialSymbolsOutlined[FILL,GRAD,opsz,wght].ttf"
	}

	text: icon
	font {
		family: "MaterialSymbolsOutlined"
	}
}
