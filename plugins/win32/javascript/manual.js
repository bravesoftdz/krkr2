/**
 * 吉里吉里クラスの取得
 * @param className 吉里吉里クラス名指定(文字列)
 * @param ... 継承している親クラスを列挙
 * @return コンストラクタメソッド
 *
 * 吉里吉里のクラスを Javascript クラスとして取得します。
 *
 * ※吉里吉里側で親クラス情報を参照生成できないため、
 * 親クラスが継承しているクラスの名前をすべて手動で列挙する必要があります。
 */
function createTJSClass(className, ...);

/**
 * 吉里吉里クラスの javascript における基底クラス構造
 * このインスタンスに対する吉里吉里側からのメンバ参照は、元の吉里吉里インスタンスのそれが呼ばれますが、
 * 存在してないメンバの場合は、missing 機能により javascript 側オブジェクトの同名メンバが参照されます。
 * 吉里吉里から呼ばれるものも直接 squirrelインスタンスのそれにに差し替える場合は
 * tjsOverride() で強制上書きをかけることができます。イベントの登録に使います。
 */
function TJSObject();

/**
 * 吉里吉里オブジェクトの有効性の確認
 * レイヤなど吉里吉里側で強制 invalidate される可能性があるオブジェクトの状況確認に使います。
 * @return valid なら true
 */
TJSObject.tjsIsValid = function();

/**
 * 吉里吉里オブジェクトの強制オーバライド処理
 * 吉里吉里インスタンスのメンバを強制的に上書きします。
 * イベントなどを javascript 側でうけたい場合に指定します
 * 値を省略した場合は自己オブジェクトを参照します
 * @param name メンバ名
 * @param value 登録する値(省略可)
 */
TJSObject.tjsOverride = function(name, value=null);
