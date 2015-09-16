public class BluetoothServerThread extends Thread {
    //サーバー側の処理
    //UUID：Bluetoothプロファイル毎に決められた値
    private final BluetoothServerSocket servSock;
    static BluetoothAdapter myServerAdapter;
    private Context mContext;
    //UUIDの生成
    public static final UUID TECHBOOSTER_BTSAMPLE_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    public String myNumber;
 
    //コンストラクタの定義
    public BluetoothServerThread(Context context,String myNum, BluetoothAdapter btAdapter){
        //各種初期化
        mContext = context;
        BluetoothServerSocket tmpServSock = null;
        myServerAdapter = btAdapter;
        myNumber = myNum;
        try{
            //自デバイスのBluetoothサーバーソケットの取得
             tmpServSock = myServerAdapter.listenUsingRfcommWithServiceRecord("BlueToothSample03", TECHBOOSTER_BTSAMPLE_UUID);
        }catch(IOException e){
            e.printStackTrace();
        }
        servSock = tmpServSock;
    }
 
    public void run(){
        BluetoothSocket receivedSocket = null;
        while(true){
            try{
                //クライアント側からの接続要求待ち。ソケットが返される。
                receivedSocket = servSock.accept();
            }catch(IOException e){
                break;
            }
 
            if(receivedSocket != null){
                //ソケットを受け取れていた(接続完了時)の処理
                //RwClassにmanageSocketを移す
                ReadWriteModel rw = new ReadWriteModel(mContext, receivedSocket, myNumber);
                rw.start();
 
                try {
                    //処理が完了したソケットは閉じる。
                    servSock.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
                break;



		public class BluetoothClientThread extends Thread {
    //クライアント側の処理
    private final BluetoothSocket clientSocket;
    private final BluetoothDevice mDevice;
    private Context mContext;
    //UUIDの生成
    public static final UUID TECHBOOSTER_BTSAMPLE_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    static BluetoothAdapter myClientAdapter;
    public String myNumber;
 
    //コンストラクタ定義
    public BluetoothClientThread(Context context, String myNum , BluetoothDevice device, BluetoothAdapter btAdapter){
        //各種初期化
        mContext = context;
        BluetoothSocket tmpSock = null;
        mDevice = device;
        myClientAdapter = btAdapter;
        myNumber = myNum;
 
        try{
            //自デバイスのBluetoothクライアントソケットの取得
            tmpSock = device.createRfcommSocketToServiceRecord(TECHBOOSTER_BTSAMPLE_UUID);
        }catch(IOException e){
            e.printStackTrace();
        }
        clientSocket = tmpSock;
    }
 
    public void run(){
        //接続要求を出す前に、検索処理を中断する。
        if
