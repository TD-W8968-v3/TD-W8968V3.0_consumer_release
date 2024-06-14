function isHexaDigit(digit) {
   var hexVals = new Array("0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                           "A", "B", "C", "D", "E", "F", "a", "b", "c", "d", "e", "f");
   var len = hexVals.length;
   var i = 0;
   var ret = false;

   for ( i = 0; i < len; i++ )
      if ( digit == hexVals[i] ) break;

   if ( i < len )
      ret = true;

   return ret;
}

function isValidKey(val, size) {
   var ret = false;
   var len = val.length;
   var dbSize = size * 2;

   if ( len == size )
      ret = true;
   else if ( len == dbSize ) {
      for ( i = 0; i < dbSize; i++ )
         if ( isHexaDigit(val.charAt(i)) == false )
            break;
      if ( i == dbSize )
         ret = true;
   } else
      ret = false;

   return ret;
}


function isValidHexKey(val, size) {
   var ret = false;
   if (val.length == size) {
      for ( i = 0; i < val.length; i++ ) {
         if ( isHexaDigit(val.charAt(i)) == false ) {
            break;
         }
      }
      if ( i == val.length ) {
         ret = true;
      }
   }

   return ret;
}


function isNameUnsafe(compareChar) {
   var unsafeString = "\"<>%\\^[]`\+\$\,='#&@.: \t";
	
   if ( unsafeString.indexOf(compareChar) == -1 && compareChar.charCodeAt(0) > 32
        && compareChar.charCodeAt(0) < 123 )
      return false; // found no unsafe chars, return false
   else
      return true;
}   

function isNameUnsafeIgnoreColon(compareChar) {
   var unsafeString = "\"<>%\\^[]`\+\$\,='#&@.\t";
   
   if ( unsafeString.indexOf(compareChar) == -1 && compareChar.charCodeAt(0) >= 32
        && compareChar.charCodeAt(0) < 123 )
      return false; // found no unsafe chars, return false
   else
      return true;
}  

// Check if a name valid
function isValidName(name) {
   var i = 0;	
   
   for ( i = 0; i < name.length; i++ ) {
      if ( isNameUnsafe(name.charAt(i)) == true )
         return false;
   }

   return true;
}

// Check if a name valid
function isValidNameIgnoreColon(name) {
   var i = 0;	
   
   for ( i = 0; i < name.length; i++ ) {
      if ( isNameUnsafeIgnoreColon(name.charAt(i)) == true )
         return false;
   }

   return true;
}

// same as is isNameUnsafe but allow spaces
function isCharUnsafe(compareChar) {
   var unsafeString = "\"<>%\\^[]`\+\$\,='#&@.:\t";
	
   if ( unsafeString.indexOf(compareChar) == -1 && compareChar.charCodeAt(0) >= 32
        && compareChar.charCodeAt(0) < 123 )
      return false; // found no unsafe chars, return false
   else
      return true;
}   

function isValidNameWSpace(name) {
   var i = 0;	
   
   for ( i = 0; i < name.length; i++ ) {
      if ( isCharUnsafe(name.charAt(i)) == true )
         return false;
   }

   return true;
}

function isSameSubNet(lan1Ip, lan1Mask, lan2Ip, lan2Mask) {

   var count = 0;
   
   lan1a = lan1Ip.split('.');
   lan1m = lan1Mask.split('.');
   lan2a = lan2Ip.split('.');
   lan2m = lan2Mask.split('.');

   for (i = 0; i < 4; i++) {
      l1a_n = parseInt(lan1a[i]);
      l1m_n = parseInt(lan1m[i]);
      l2a_n = parseInt(lan2a[i]);
      l2m_n = parseInt(lan2m[i]);
      if ((l1a_n & l1m_n) == (l2a_n & l2m_n))
         count++;
   }
   if (count == 4)
      return true;
   else
      return false;
}


function isValidIpAddress(address) {
   var i = 0;

   if ( address == '0.0.0.0' ||
        address == '255.255.255.255' )
      return false;

   addrParts = address.split('.');
   if ( addrParts.length != 4 ) return false;
   for (i = 0; i < 4; i++) {
      if (isNaN(addrParts[i]) || addrParts[i] =="")
         return false;
      num = parseInt(addrParts[i]);
      if ( num < 0 || num > 255 )
         return false;
	  if ((i == 0)&&(num >= 224))
	  	 return false;
	  if ((i == 0)&&(num == 127))
	  	 return false;
	  if ((i == 0)&&(num == 169))
	  	 return false;
	  if ((i == 3)&&(num == 0))
		 return false;
	  if ((i == 3)&&(num == 255))
		 return false;
   }
   return true;
}
function isValidIpAddressForStRoute(address) {
   var i = 0;

   if ( address == '0.0.0.0' ||
        address == '255.255.255.255' )
      return false;

   addrParts = address.split('.');
   if ( addrParts.length != 4 ) return false;
   for (i = 0; i < 4; i++) {
      if (isNaN(addrParts[i]) || addrParts[i] =="")
         return false;
      num = parseInt(addrParts[i]);
      if ( num < 0 || num > 255 )
         return false;
	  if ((i == 0)&&(num >= 224))
	  	 return false;
	  if ((i == 0)&&(num == 127))
            return false;
        }
   return true;
    }
function isValidIpAddress6(address) {
   var i = 0, num = 0;

   addrParts = address.split(':');
   if (addrParts.length < 3 || addrParts.length > 8)
            return false;
   for (i = 0; i < addrParts.length; i++) {
      if ( addrParts[i] != "" )
         num = parseInt(addrParts[i], 16);
      if ( i == 0 ) {
//         if ( (num & 0xf000) == 0xf000 )
//            return false;	//can not be link-local, site-local or multicast address
  } 
      else if ( (i + 1) == addrParts.length) {
         if ( num == 0 || num == 1)
            return false;	//can not be unspecified or loopback address
  }
      if ( num != 0 )
         break;
  } 
   return true;
}

function isValidPrefixLength(prefixLen) {
   var num;

   num = parseInt(prefixLen);
   if (num <= 0 || num > 128)
      return false;
   return true;
}

function areSamePrefix(addr1, addr2) {
   var i, j;
   var a = [0, 0, 0, 0, 0, 0, 0, 0];
   var b = [0, 0, 0, 0, 0, 0, 0, 0];

   addr1Parts = addr1.split(':');
   if (addr1Parts.length < 3 || addr1Parts.length > 8)
      return false;
   addr2Parts = addr2.split(':');
   if (addr2Parts.length < 3 || addr2Parts.length > 8)
      return false;
   j = 0;
   for (i = 0; i < addr1Parts.length; i++) {
      if ( addr1Parts[i] == "" ) {
		 if ((i != 0) && (i+1 != addr1Parts.length)) {
			j = j + (8 - addr1Parts.length + 1);
		 }
		 else {
		    j++;
		 }
	  }
	  else {
         a[j] = parseInt(addr1Parts[i], 16);
		 j++;
	  }
   }
   j = 0;
   for (i = 0; i < addr2Parts.length; i++) {
      if ( addr2Parts[i] == "" ) {
		 if ((i != 0) && (i+1 != addr2Parts.length)) {
			j = j + (8 - addr2Parts.length + 1);
		 }
		 else {
		    j++;
		 }
	  }
	  else {
         b[j] = parseInt(addr2Parts[i], 16);
		 j++;
	  }
   }
   //only compare 64 bit prefix
   for (i = 0; i < 4; i++) {
      if (a[i] != b[i]) {
	     return false;
	  }
   }
   return true;
}

function getLeftMostZeroBitPos(num) {
   var i = 0;
   var numArr = [128, 64, 32, 16, 8, 4, 2, 1];

   for ( i = 0; i < numArr.length; i++ )
      if ( (num & numArr[i]) == 0 )
         return i;

   return numArr.length;
}

function getRightMostOneBitPos(num) {
   var i = 0;
   var numArr = [1, 2, 4, 8, 16, 32, 64, 128];

   for ( i = 0; i < numArr.length; i++ )
      if ( ((num & numArr[i]) >> i) == 1 )
         return (numArr.length - i - 1);

   return -1;
}
function getLeftMostOneBitPos(num)
{
	var i = 0;
	var numArr = [128, 64, 32, 16, 8, 4, 2, 1];
	for ( i = 0; i < numArr.length; i++ )
		if ( (num >= numArr[i]) )
			return i;

	return numArr.length;
}
function getRightMostZeroBitPos(num)
{
	var i = 0;
	var numArr = [1, 2, 4, 8, 16, 32, 64, 128];
	for ( i = 0; i < numArr.length; i++ )
		if ( ((num & numArr[i]) >> i) == 0 )
			return (numArr.length - i - 1);

	return 8;
}
function isValidSubnetMask(mask) {
   var i = 0, num = 0;
   var zeroBitPos = 0, oneBitPos = 0;
   var zeroBitExisted = false;

   if ( mask == '0.0.0.0' )
      return false;

   maskParts = mask.split('.');
   if ( maskParts.length != 4 ) return false;

   for (i = 0; i < 4; i++) {
      if ( isNaN(maskParts[i]) == true )
         return false;
      num = parseInt(maskParts[i]);
      if ( num < 0 || num > 255 )
         return false;
      if ( zeroBitExisted == true && num != 0 )
         return false;
      zeroBitPos = getLeftMostZeroBitPos(num);
      oneBitPos = getRightMostOneBitPos(num);
      if ( zeroBitPos < oneBitPos )
         return false;
      if ( zeroBitPos < 8 )
         zeroBitExisted = true;
   }

   return true;
}
//added by xieping
function isValidHostIp(ip,mask)
{
//	alert("isValidHostIp!!");
//	alert(ip);
//	alert(mask);
	if ( (isValidIpAddress(ip) == false) || (isValidSubnetMask(mask) == false) )
		return false;

	maskParts = mask.split('.');
	ipParts = ip.split('.');

	//验证网络地址合法
	for( i = 0; i < 4; i++)
	{
		numIp = parseInt(ipParts[i]);
		numMask = parseInt(maskParts[i]);
		if( numMask == 255 && numIp < 255 )
		{
			break;
		}
		if( numMask < 255)
		{
			zeroBitPosMask = getLeftMostZeroBitPos(numMask);
			zeroBitPosIp = getLeftMostZeroBitPos(numIp);
			if ( zeroBitPosIp >= zeroBitPosMask)//网络段全一
			{
				return false;
			}
			oneBitPosIp = getLeftMostOneBitPos(numIp);
			if ( oneBitPosIp >= zeroBitPosMask)//网络段全零
			{
				return false;
			}
		}
	}
	//验证地址段合法
	for ( i = 3;i >= 0; i--)
	{
		numIp = parseInt(ipParts[i]);
		numMask = parseInt(maskParts[i]);
		if (numMask == 0 && numIp != 0)
		{
			break;
		}
		if (numMask != 0)
		{
			zeroBitPosIp = getRightMostZeroBitPos(numIp);
			oneBitPosMask = getRightMostOneBitPos(numMask);
			if (zeroBitPosIp <= oneBitPosMask)
			{
				if( i == 3)//地址段全一
				{
					return false;
				}
			}
			oneBitPosIp = getRightMostOneBitPos(numIp);
			if (oneBitPosIp <= oneBitPosMask)
			{
				if( i == 3)//地址段全零
				{	
					return false;
				}
			}
		}
	}
	return true;
}
//end added
function isValidPort(port) {
   var fromport = 0;
   var toport = 100;

   portrange = port.split(':');
   if ( portrange.length < 1 || portrange.length > 2 ) {
       return false;
   }
   fromport = parseInt(portrange[0]);
   if ( isNaN(fromport) )
       return false;
   
   if ( portrange.length > 1 ) {
       toport = parseInt(portrange[1]);
       if ( isNaN(toport) )
          return false;
       if ( toport <= fromport )
           return false;      
   }
   
   if ( fromport < 1 || fromport > 65535 || toport < 1 || toport > 65535 )
       return false;
   
   return true;
}

function isValidNatPort(port) {
   var fromport = 0;
   var toport = 100;

   portrange = port.split('-');
   if ( portrange.length < 1 || portrange.length > 2 ) {
       return false;
   }
   if ( isNaN(portrange[0]) )
       return false;
   fromport = parseInt(portrange[0]);

   if ( portrange.length > 1 ) {
       if ( isNaN(portrange[1]) )
          return false;
       toport = parseInt(portrange[1]);
       if ( toport <= fromport )
           return false;
   }

   if ( fromport < 1 || fromport > 65535 || toport < 1 || toport > 65535 )
       return false;

   return true;
}
/*
function isValidMacAddress(address) {
   var c = '';
   var num = 0;
   var i = 0, j = 0;
   var zeros = 0;

   addrParts = address.split(':');
   if ( addrParts.length != 6 ) return false;

   for (i = 0; i < 6; i++) {
      if ( addrParts[i] == '' )
         return false;
      for ( j = 0; j < addrParts[i].length; j++ ) {
         c = addrParts[i].toLowerCase().charAt(j);
         if ( (c >= '0' && c <= '9') ||
              (c >= 'a' && c <= 'f') )
            continue;
         else
            return false;
      }

      num = parseInt(addrParts[i], 16);
      if ( num == NaN || num < 0 || num > 255 )
         return false;
      if ( num == 0 )
         zeros++;
   }
   if (zeros == 6)
      return false;

   return true;
}
*/
function isValidMacAddress(address) {

	var hexnumIndex = '0123456789abcdefABCDEF';
	var reg = /(([0-9, a-f, A-F]){2}:){5}([0-9, a-f, A-F]){2}/ig ;
	var j = 0;

	if (address.length != 17 || address.match(reg) == null)
	{
		//alert(" the address " + address + " is invalid, please retry");
		return false;
	}

	j = hexnumIndex.indexOf(address.charAt(1));
	if ( j & 0x01 != 0 )
	{
		//alert("the address " + address + " is invalid, please retry");
            return false;
      }

	if ( address == "00:00:00:00:00:00")
	{
		//alert("the address " + address + " is valid, please retry");
         return false;
   }

   return true;
}

function isValidSubnetMaskForStRoute(mask, ip)  {
   var i = 0, num = 0;
   var zeroBitPos = 0, oneBitPos = 0;
   var zeroBitExisted = false;
   var host = 0;        //mark if the host part of the ip address is zero
   var net = 0;     //mark if the net part of the ip address is zero
   var hostAllOne = 1;  //mark if all of the host part bit are one

   if ( mask == '0.0.0.0' )
   {
          alert( mask + " is Invalid Subnet Mask, Please reset!");
      return false;
   }

   maskParts = mask.split('.');
   ipParts = ip.split('.');
   if ( maskParts.length != 4 )
   {
          alert( mask + " is Invalid Subnet Mask, Please reset!");
      return false;
   }
   if ( ipParts.length != 4 )
   {
          alert( mask + " is Invalid Subnet Mask, Please reset!");
      return false;
   }

   for (i = 0; i < 4; i++) {
      if ( isNaN(maskParts[i]) == true )
          {
                  alert( mask + " is Invalid Subnet Mask, Please reset!");
         return false;
      }

      num = parseInt(maskParts[i]);

      if ( num < 0 || num > 255 )
          {
                  alert( mask + " is Invalid Subnet Mask, Please reset!");
         return false;
      }

      if ( zeroBitExisted == true && num != 0 )
          {
                  alert( mask + " is Invalid Subnet Mask, Please reset!");
         return false;
      }

      zeroBitPos = getLeftMostZeroBitPos(num);
      oneBitPos = getRightMostOneBitPos(num);
                              

      if ( zeroBitPos < oneBitPos )
          {
                  alert( mask + " is Invalid Subnet Mask, Please reset!");
         return false;
      }

      if ( zeroBitPos < 8 )
         zeroBitExisted = true;

          if (hostAllOne != 0)
      {
         hostAllOne = (((~num) & parseInt(ipParts[i])) == (255 & (~num)))?1:0;
      }

          host = host?host:((~num) & parseInt(ipParts[i]));
          net  = net?net:(num & parseInt(ipParts[i]));
   }

   if (host != 0)
   {
      alert("The combination of IP address and subnet mask is invalid. All of the bits in the host address portion of the IP address must be set to 0. Please enter a valid combination of IP address and subnet mask.");
          return false;
   }

   if (net == 0)
   {
      alert("The combination of IP address and subnet mask is invalid. All of the bits in the network address portion of the IP address are set to 0. Please enter a valid combination of IP address and subnet mask.");
          return false;
   }

   return true;
}

function isValidMacMask(mask) {
   var c = '';
   var num = 0;
   var i = 0, j = 0;
   var zeros = 0;
   var zeroBitPos = 0, oneBitPos = 0;
   var zeroBitExisted = false;

   maskParts = mask.split(':');
   if ( maskParts.length != 6 ) return false;

   for (i = 0; i < 6; i++) {
      if ( maskParts[i] == '' )
         return false;
      for ( j = 0; j < maskParts[i].length; j++ ) {
         c = maskParts[i].toLowerCase().charAt(j);
         if ( (c >= '0' && c <= '9') ||
              (c >= 'a' && c <= 'f') )
            continue;
         else
            return false;
      }

      num = parseInt(maskParts[i], 16);
      if ( num == NaN || num < 0 || num > 255 )
         return false;
      if ( zeroBitExisted == true && num != 0 )
         return false;
      if ( num == 0 )
         zeros++;
      zeroBitPos = getLeftMostZeroBitPos(num);
      oneBitPos = getRightMostOneBitPos(num);
      if ( zeroBitPos < oneBitPos )
         return false;
      if ( zeroBitPos < 8 )
         zeroBitExisted = true;
   }
   if (zeros == 6)
      return false;

   return true;
}

var hexVals = new Array("0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
              "A", "B", "C", "D", "E", "F");
var unsafeString = "\"<>%\\^[]`\+\$\,'#&";
// deleted these chars from the include list ";", "/", "?", ":", "@", "=", "&" and #
// so that we could analyze actual URLs

function isUnsafe(compareChar)
// this function checks to see if a char is URL unsafe.
// Returns bool result. True = unsafe, False = safe
{
   if ( unsafeString.indexOf(compareChar) == -1 && compareChar.charCodeAt(0) > 32
        && compareChar.charCodeAt(0) < 123 )
      return false; // found no unsafe chars, return false
   else
      return true;
}

function decToHex(num, radix)
// part of the hex-ifying functionality
{
   var hexString = "";
   while ( num >= radix ) {
      temp = num % radix;
      num = Math.floor(num / radix);
      hexString += hexVals[temp];
   }
   hexString += hexVals[num];
   return reversal(hexString);
}

function reversal(s)
// part of the hex-ifying functionality
{
   var len = s.length;
   var trans = "";
   for (i = 0; i < len; i++)
      trans = trans + s.substring(len-i-1, len-i);
   s = trans;
   return s;
}

function convert(val)
// this converts a given char to url hex form
{
   return  "%" + decToHex(val.charCodeAt(0), 16);
}


function encodeUrl(val)
{
   var len     = val.length;
   var i       = 0;
   var newStr  = "";
   var original = val;

   for ( i = 0; i < len; i++ ) {
      if ( val.substring(i,i+1).charCodeAt(0) < 255 ) {
         // hack to eliminate the rest of unicode from this
         if (isUnsafe(val.substring(i,i+1)) == false)
            newStr = newStr + val.substring(i,i+1);
         else
            newStr = newStr + convert(val.substring(i,i+1));
      } else {
         // woopsie! restore.
         alert ("Found a non-ISO-8859-1 character at position: " + (i+1) + ",\nPlease eliminate before continuing.");
         newStr = original;
         // short-circuit the loop and exit
         i = len;
      }
   }

   return newStr;
}

var markStrChars = "\"'";

// Checks to see if a char is used to mark begining and ending of string.
// Returns bool result. True = special, False = not special
function isMarkStrChar(compareChar)
{
   if ( markStrChars.indexOf(compareChar) == -1 )
      return false; // found no marked string chars, return false
   else
      return true;
}

// use backslash in front one of the escape codes to process
// marked string characters.
// Returns new process string
function processMarkStrChars(str) {
   var i = 0;
   var retStr = '';

   for ( i = 0; i < str.length; i++ ) {
      if ( isMarkStrChar(str.charAt(i)) == true )
         retStr += '\\';
      retStr += str.charAt(i);
   }

   return retStr;
}

// Web page manipulation functions

function showhide(element, sh)
{
    var status;
    if (sh == 1) {
        status = "block";
    }
    else {
        status = "none"
    }
    
	if (document.getElementById)
	{
		// standard
		document.getElementById(element).style.display = status;
	}
	else if (document.all)
	{
		// old IE
		document.all[element].style.display = status;
	}
	else if (document.layers)
	{
		// Netscape 4
		document.layers[element].display = status;
	}
}

/* added by wwj from v4, 2013-04-22 */
function showhide_tr(id, cnt, show) {
	var display = show ? (navigator.appName == 'Microsoft Internet Explorer' ?
		'block' : 'table-row') : 'none';
		var tr = document.getElementById(id);
		var trs = tr.parentNode.rows;
		var i = tr.rowIndex;
	if (typeof cnt == 'string') {
		cnt = document.getElementById(cnt).rowIndex - i + 1;
	}
		while (tr && --cnt >= 0) {
			tr.style.display = display;
			tr = trs[++i];
		}
}

// Load / submit functions

function getSelect(item)
{
	var idx;
	if (item.options.length > 0) {
	    idx = item.selectedIndex;
	    return item.options[idx].value;
	}
	else {
		return '';
    }
}

function setSelect(item, value)
{
	for (i=0; i<item.options.length; i++) {
        if (item.options[i].value == value) {
        	item.selectedIndex = i;
        	break;
        }
    }
}

function setCheck(item, value)
{
    if ( value == '1' ) {
         item.checked = true;
    } else {
         item.checked = false;
    }
}

function setInnerHtml(item,value) {
	document.getElementById(item).innerHTML=value;
}

function setDisable(item, value)
{
    if ( value == 1 || value == '1' ) {
         item.disabled = true;
    } else {
         item.disabled = false;
    }     
}

function submitText(item)
{
	return '&' + item.name + '=' + item.value;
}

function submitSelect(item)
{
	return '&' + item.name + '=' + getSelect(item);
}


function submitCheck(item)
{
	var val;
	if (item.checked == true) {
		val = 1;
	} 
	else {
		val = 0;
	}
	return '&' + item.name + '=' + val;
}

/* added by yanglei, 2011-12-26 */
function go(loc, sessionKey) {
	if (sessionKey) {
		loc += '&sessionKey='+sessionKey;
	}
	var code = 'location="' + loc + '"';
	eval(code);
}

function stripStr(str) {
	return str.replace(/^\s+|\s+$/g, '');
}

/* added by yanglei, 2012-2-16 */
function radioCheck(radio, value) {
	var i = radio.length;
	if (value  === undefined) { /* get value */
		while (--i >= 0) {
			if (radio[i].checked) { return radio[i].value; }
		}
	} else { /* set value */
		while (--i >= 0) {
			if (radio[i].value == value) { radio[i].checked = true; return; }
		}
	}
}

function alertAndFocus(ctl, msg)
{
	alert(msg);
	ctl.focus();
}
