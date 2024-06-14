function Folder(folderDescription, hreference) //constructor
{
  //constant data
  this.desc = folderDescription
  this.hreference = hreference
  this.id = -1
  this.navObj = 0
  this.children = new Array
  this.nChildren = 0
  this.level = 0
  this.leftSideCoded = ""

  //dynamic data
  this.isOpen = false
  this.isRendered = 0
  this.currentTitle = "plus.gif";

  //methods
  this.initialize = initializeFolder
  this.setState = setStateFolder
  this.addChild = addChild
  this.createIndex = createEntryIndex
  this.escondeBlock = escondeBlock
  this.esconde = escondeFolder
  this.mostra = mostra
  this.renderOb = drawFolder
  this.totalHeight = totalHeight
  this.subEntries = folderSubEntries
  this.blockStartHTML = blockStartHTML
  this.blockEndHTML = blockEndHTML
}

function initializeFolder(level, leftSide)
{
  var j=0
  var i=0
  nc = this.nChildren

  this.createIndex()
  this.level = level
  this.leftSideCoded = leftSide

  if (browserVersion == 0)
    this.isOpen=true;

  if (level>0)
    leftSide = leftSide + "0"

  if (nc > 0)
  {
    level = level + 1
    for (i=0 ; i < this.nChildren; i++)
    {
        this.children[i].initialize(level, leftSide)
    }
  }
}

function drawFolder()
{
	var docW = ""
  var blank = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
	var leftSide = leftSideHTML(this.leftSideCoded);
	var varid = this.id + 100;

	this.isRendered = 1

	if (browserVersion == 2) {
		if (!doc.yPos)
			doc.yPos=10
	}
	this.currentTitle = this.nChildren ? "plus.gif" : "pw.gif";
	docW = this.blockStartHTML("folder");
	if (this.level > 0) {
		docW = docW + "<tr>"
/*		docW = docW + "<div>" + this.currentTitle + "</div>" */
		if (this.level == 1 ) {
			docW = docW + "<td class='menuCell' valign=middle nowrap width=100%>"
			docW = docW + "<a class='menuLink'"+ "id = '" + this.id +"' valign=middle href='" + this.hreference + "' TARGET=\"basefrm\"" 
	}
	else if(this.level == 2){
			docW = docW + "<td class='subMenuCell' valign=middle nowrap width=100%>"
			docW = docW + "<a class='submenuLink'"+ "id = '" + this.id +"' valign=middle href='" + this.hreference + "' TARGET=\"basefrm\""
	}
	else if(this.level > 2){
			docW = docW + "<td class='smallMenuCell' valign=middle nowrap width=100%>"
			docW = docW + "<a class='smallmenuLink'"+ "id = '" + this.id +"'	valign=middle href='" + this.hreference + "' TARGET=\"basefrm\""
		}

		if (browserVersion > 0) {
			docW = docW + "onClick='javascript:clickOnFolder("+this.id+")'"
		}
		
		if (this.level == 1) {
	docW = docW + ">" + leftSide + "&nbsp;&nbsp;&nbsp;" + this.desc + blank + "</a>" 
			docW = docW + "</td>"
	}
	else if(this.level == 2){	
		docW = docW + ">" + leftSide + "" + "<img id='"+ varid  +"' src = " + this.currentTitle+ " border=0></img>" + this.desc + blank + "</a>" 
			docW = docW + "</td>"
	}
	else{
		docW = docW + ">" + "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" + "<img id='"+ varid +"' src = " + this.currentTitle+ " border=0></img>" + this.desc + "&nbsp;&nbsp;" + "</a>" 
			docW = docW + "</td>"
		}
	}
	docW = docW + this.blockEndHTML()
	doc.write(docW)

  if (browserVersion == 1)
		this.navObj = getElById("folder"+this.id)
  else if (browserVersion == 2)
  {
		this.navObj = doc.layers["folder"+this.id]
		doc.yPos = doc.yPos+this.navObj.clip.height
	}
	if (isOpera && (docW = getElById(this.id))) docW.style.height = '80%';
}

function setStateFolder(isOpen)
{
  var subEntries
  var totalHeight
  var fIt = 0
  var i=0
  var currentOpen
  if (isOpen == this.isOpen)
    return

  if (browserVersion == 2)
  {
    totalHeight = 0
    for (i=0; i < this.nChildren; i++)
      totalHeight = totalHeight + this.children[i].navObj.clip.height
      subEntries = this.subEntries()
    if (this.isOpen)
      totalHeight = 0 - totalHeight
    for (fIt = this.id + subEntries + 1; fIt < nEntries; fIt++)
      indexOfEntries[fIt].navObj.moveBy(0, totalHeight)
  }
  this.isOpen = isOpen;

  propagateChangesInState(this)
}

function propagateChangesInState(folder)
{
  var i=0

  //Propagate changes
  for (i=folder.nChildren-1; i>=0; i--)
    if (folder.isOpen)
      folder.children[i].mostra()
    else
      folder.children[i].esconde()
}

function escondeFolder()
{
  this.escondeBlock()
  this.setState(0)
}

function addChild(childNode)
{
  this.children[this.nChildren] = childNode
  this.nChildren++
  return childNode
}

function folderSubEntries()
{
  var i = 0
  var se = this.nChildren

  for (i=0; i < this.nChildren; i++){
    if (this.children[i].children) //is a folder
      se = se + this.children[i].subEntries()
  }

  return se
}

// Definition of class Item (a document or link inside a Folder)
function Item(itemDescription, itemLink) // Constructor
{
  // constant data
  this.desc = itemDescription
  this.link = itemLink
  this.id = -1 //initialized in initalize()
  this.navObj = 0 //initialized in render()
  this.isRendered = 0
  this.level = 0
  this.leftSideCoded = ""
  this.nChildren = 0

  // methods
  this.initialize = initializeItem
  this.createIndex = createEntryIndex
  this.escondeBlock = escondeBlock
  this.esconde = escondeBlock
  this.mostra = mostra
  this.renderOb = drawItem
  this.totalHeight = totalHeight
  this.blockStartHTML = blockStartHTML
  this.blockEndHTML = blockEndHTML
}

function initializeItem(level, leftSide)
{
  this.createIndex()
  this.level = level
  this.leftSideCoded = leftSide
}

function drawItem()
{
	var leftSide = leftSideHTML(this.leftSideCoded)
	var docW = ""
  var blank = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
	this.isRendered = 1

	docW = this.blockStartHTML("item")
	docW = docW + "<tr>"

  
	/* docW = docW + "<div>" + this.desc + "</div>" */
	if (this.level == 1) {
		docW = docW + "<td class = 'menuCell' valign=middle nowrap width=100%>"
		docW = docW + "<a class='menuLink'"+ "id = '" + this.id +"' valign=middle href=" + this.link +" onClick = 'javascript:clickOnFolder(" +this.id + ")'"
	docW = docW + ">" + leftSide + "&nbsp;&nbsp;&nbsp;" + this.desc + blank + "</a>"
  }
  else if(this.level == 2){
		docW = docW + "<td class = 'subMenuCell' valign=middle nowrap width=100%>"
		docW = docW + "<a class='submenuLink'"+ "id = '" + this.id +"' valign=middle href=" + this.link +" onClick = 'javascript:clickOnFolder(" +this.id + ")'"
	docW = docW + ">" + leftSide + "" + "<img src = \"pw.gif\" border=0></img>" + this.desc + blank + "</a>"
  }
  else if(this.level == 3){
		docW = docW + "<td class = 'smallMenuCell' valign=middle nowrap width=100%>"
		docW = docW + "<a class='smallmenuLink'"+ "id = '" + this.id +"' valign=middle href=" + this.link +" onClick = 'javascript:clickOnFolder(" +this.id + ")'"
	docW = docW + ">" + "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" + "<img src = \"pw.gif\" border=0></img>" + this.desc + blank + "</a>"
  }
  else{
		docW = docW + "<td class = 'smallMenuCell' valign=bottom nowrap width=100%>"
		docW = docW + "<a class='smallmenuLink'"+ "id = '" + this.id +"' valign=middle href=" + this.link +" onClick = 'javascript:clickOnFolder(" +this.id + ")'"
	docW = docW + ">" + "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" + "<img src = \"pw.gif\" border=0></img>" + this.desc + blank + "</a>"
	}
//	docW = docW + "<a class='menuLink' href=" + this.link + ">" + leftSide + this.desc + "&nbsp;&nbsp;" + "</a>"

  
/*	docW = docW + ">" + leftSide + "&nbsp;&nbsp;" + "<img src = \"pw.gif\" border=0></img>" + this.desc + "&nbsp;&nbsp;" + "</a>"*/
	docW = docW + "</td>"
	docW = docW + this.blockEndHTML()
	doc.write(docW)

	if (browserVersion == 2) {
		this.navObj = doc.layers["item"+this.id]
		doc.yPos=doc.yPos+this.navObj.clip.height
	} else if (browserVersion != 0) {
		this.navObj = getElById("item"+this.id)
	}
	if (isOpera && (docW = getElById(this.id))) docW.style.height = '80%';
}

// Methods common to both objects (pseudo-inheritance)
function escondeBlock()
{
  if (browserVersion == 1) {
    this.navObj.style.display = "none"
  } else {
    this.navObj.visibility = "hiden"
  }
}

function mostra()
{
  if (!this.isRendered)
     this.renderOb()
  else
    if (browserVersion == 1)
      this.navObj.style.display = "block"
    else
      this.navObj.visibility = "show"
}

function blockStartHTML(idprefix) {
  var idParam = "id='" + idprefix + this.id + "'"
  var docW = ""

  if (browserVersion == 2) {
    docW = "<layer "+ idParam + " top=" + doc.yPos + " >"
  } else if (browserVersion == 1) {
    docW = "<div " + idParam + " >"
  }

  docW = docW + "<table border=0 cellspacing=1 cellpadding=0 style='width:100%'>"

  return docW
}

function blockEndHTML() {
  var docW = ""

  docW = "</table>"

  if (browserVersion == 2)
    docW = docW + "</layer>"
  else if (browserVersion == 1)
    docW = docW + "</div>"

  return docW
}

function createEntryIndex()
{
  this.id = nEntries
  indexOfEntries[nEntries] = this
  nEntries++
}

// total height of subEntries open
function totalHeight() //used with browserVersion == 2
{
  var h = this.navObj.clip.height
  var i = 0

  if (this.isOpen) //is a folder and _is_ open
    for (i=0 ; i < this.nChildren; i++)
      h = h + this.children[i].totalHeight()

  return h
}

function leftSideHTML(leftSideCoded) {
    var i;
    var retStr = "";

    for (i=0; i<leftSideCoded.length; i++)
    {
        if (leftSideCoded.charAt(i) == "0")
        {
            retStr = retStr + "&nbsp;&nbsp;&nbsp;"
        }
    }
    return retStr
}

// Events
function clickOnFolder(folderId)
{
	var clicked = indexOfEntries[folderId];
	var e;

	// open only current folder, and close other siblings
	//alert("in clickOnFolder")
	var folder;
    for ( i = nEntries-1; i > 0; i-- )
	{
		folder = indexOfEntries[i];
		/*add by zxm for change bgcolor*/
        	if (folder.id != folderId)
        	{
        		if ( folder.level == 1)
				{
				document.getElementById(folder.id).style.backgroundColor="#424242";
				}
				else if ( folder.level == 2)
				{
				document.getElementById(folder.id).style.backgroundColor="#606060";
				}
				else
				{	
				document.getElementById(folder.id).style.color="#ffffff";
			}
		}
		/*end add by zxm for change bgcolor*/
		if ( folder.level >= clicked.level && folder.isOpen == true)
		{
			folder.setState(false);
			folder.currentTitle = "plus.gif";
			document.getElementById(folder.id).src = folder.currentTitle;
			/*add by zxm for change bgcolor*/
				if (folder.level > 1)
				document.getElementById(folder.id + 100).src = folder.currentTitle;
			/*end add by zxm for change bgcolor*/
		}
	}
	try
	{	
		/*alert("in clickOnFolder");
		clicked.currentTitle = "minus.gif";
		alert("clicked.currentTitle = 'plus.gif'");*/
		if ( clicked.currentTitle == "plus.gif" )
		{
			clicked.setState(true);
			clicked.currentTitle = "minus.gif";
			document.getElementById(clicked.id).src = clicked.currentTitle;
			/*add by zxm for change bgcolor*/
			if (clicked.level > 1)
				document.getElementById(clicked.id + 100).src = clicked.currentTitle;
			/*end add by zxm for change bgcolor*/
		}
		/*add by zxm for change bgcolor*/
		if ( clicked.level == 1)
		{
			document.getElementById(clicked.id).style.backgroundColor="#00648c";
		}
		else if ( clicked.level == 2)
		{
			document.getElementById(clicked.id).style.backgroundColor="#0099cc";
		}
		else
		{
			document.getElementById(clicked.id).style.color="#00ccff";
		}
		/*end add by zxm for change bgcolor*/
	}catch(e)
	{
		;
	}
}

// Auxiliary Functions
function gFld(description, hreference)
{
  folder = new Folder(description, hreference)
  return folder
}

function gLnk(optionFlags, description, linkData)
{
  var fullLink = "";
  var targetFlag = "";
  var target = "";
  var protocol = "";

  targetFlag = optionFlags.charAt(0)
  if (targetFlag=="R")
    target = "basefrm"
  if (targetFlag=="S")
    target = "_self"

  fullLink = "'" + linkData + "' target=" + target

  linkItem = new Item(description, fullLink)
  return linkItem
}

function insFld(parentFolder, childFolder)
{
  return parentFolder.addChild(childFolder)
}

function insDoc(parentFolder, document)
{
  return parentFolder.addChild(document)
}

function renderAllTree(nodeObj) {
  var i=0;
  nodeObj.renderOb()
  for (i=0 ; i < nodeObj.nChildren; i++) 
      renderAllTree(nodeObj.children[i])
}

function hideWholeTree(nodeObj, hideThisOne, nodeObjMove) {
  var i=0;
  var heightContained=0;
  var childrenMove=nodeObjMove;

  if (hideThisOne)
    nodeObj.escondeBlock()

  if (browserVersion == 2)
    nodeObj.navObj.moveBy(0, 0-nodeObjMove)

  for (i=0 ; i < nodeObj.nChildren; i++) {
    heightContainedInChild = hideWholeTree(nodeObj.children[i], true, childrenMove)
    if (browserVersion == 2) {
      heightContained = heightContained + heightContainedInChild + nodeObj.children[i].navObj.clip.height
      childrenMove = childrenMove + heightContainedInChild
    }
  }

  return heightContained;
}

function getElById(idVal) {
  if (document.getElementById != null)
    return document.getElementById(idVal)
  if (document.all != null)
    return document.all[idVal]
  
  alert("Problem getting element by id")
  return null
}

//Other variables
indexOfEntries = new Array
nEntries = 0
browserVersion = 0 
doc = document
doc.yPos = 0
var isOpera = false;

// Main function
function initializeDocument()
{
  xbDetectBrowser();
  
  foldersTree.initialize(0, "0")
  renderAllTree(foldersTree);
  if (browserVersion != 0)
    hideWholeTree(foldersTree, false, 0)

  if (browserVersion == 2)
    doc.write("<layer top=" + indexOfEntries[nEntries-1].navObj.top + ">&nbsp;</layer>")

  if (browserVersion != 0) {
    var clickedFolder
    clickedFolder = indexOfEntries[0]
    clickedFolder.setState(true)
  }
}

function xbDetectBrowser()
{
  var oldOnError = window.onerror;

  window.onerror = null;

  // work around bug in xpcdom Mozilla 0.9.1
  window.saveNavigator = window.navigator;

  var ua = window.navigator.userAgent.toLowerCase();

  if ((ua.indexOf('msie') != -1) || (ua.indexOf('konqueror') != -1) || (ua.indexOf('gecko') != -1))
  {
    browserVersion = 1;
  }
  else if ((ua.indexOf('mozilla') !=-1) && (ua.indexOf('spoofer')==-1) && (ua.indexOf('compatible') == -1) && (ua.indexOf('opera')==-1)&& (ua.indexOf('webtv')==-1) && (ua.indexOf('hotjava')==-1))
  {
    browserVersion = 2;
  }
  else if (ua.indexOf('presto') != -1)
  {
	isOpera = true; browserVersion = 1;
  }
  else
  {
    browserVersion = 0;
  }

  window.onerror = oldOnError;
}
