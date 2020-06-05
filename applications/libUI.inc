function createWindow(locationX,locationY,sizeX,sizeY,title,titleSize) {
	local i;
	for (i=0; $ptr8u[title, i]; i++)
		$uf0($ptr8u[title, i]);
	$uf1(locationX,locationY,sizeX,sizeY,titleSize);
	$uf0(); // call with no args to reset text buffer
}

function printWindow(locationX,locationY,windowNumber,text,textSize,color) {
	local i;
	for (i=0; $ptr8u[text, i]; i++)
		$uf0($ptr8u[text, i]);
	$uf2(locationX,locationY,windowNumber,textSize,color);
	$uf0(); // call with no args to reset text buffer
}

function printWindowInt(locationX,locationY,windowNumber,num,color) {
	$uf10(locationX,locationY,windowNumber,num,color);
}

function setWindowTile(locationX,locationY,windowNumber,tile) {
	$uf5(locationX,locationY,windowNumber,tile);
}

function setWindowRamTile(locationX,locationY,windowNumber,tile) {
	$uf6(locationX,locationY,windowNumber,tile);
}

function plotRamTile(locationX,locationY,ramtile,color) {
	$uf7(locationX,locationY,ramtile,color);
}

function clearWindow(windowNumber,tile) {
	$uf4(windowNumber,tile);
}

function playPatch(patch,volume) {
	$uf8(patch,volume);
}

function playNote(channel,patch,note,volume) {
	$uf9(channel,patch,note,volume);
}
