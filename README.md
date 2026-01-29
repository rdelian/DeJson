<p align="center">
	<img src="https://github.com/rdelian/DeJson/blob/main/Resources/Icon128.png" />
	<p align="center"><b>DeJson</b> - An easy way to convert almost anything to JSON and back to UE</p>
</p>

----

![alt text](https://i.imgur.com/WZwpF0Y.png)

1. Use `DeJsonField` to convert anything to a _JSON value_ and build your JSON Object or String.
You can pass primitive types, structs, arrays, maps, sets, etc.

2. You can `Convert Json String To Struct` easily, without writing a custom parser for each struct, just break to your struct.

_\*This plugin is intended to be used along the "Json Blueprint Utilities" unreal plugin._

## How to install
1. Clone this repository into `./YourProject/Plugins/` folder
2. Build the game

![install](https://i.imgur.com/PWLxDCK.png)

## Examples
### Build a JSON String
![basic example](https://i.imgur.com/6ZDxGwn.png)

### Struct → JSON String → Struct
![struct to json string to struct](https://i.imgur.com/3ZolWTL.png)

## Use case
### Send data to backend
![backend example](https://i.imgur.com/eaFJK6d.png)

- Using `Struct -> JSON String` & `JSON String -> Struct` you can send and receive data from your backend in a very easy way, keeping your code clean and maintainable with strong typing.
- Or even writing/reading JSON files from disk for various system.