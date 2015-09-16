import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;

import javax.bluetooth.BluetoothStateException;
import javax.bluetooth.DeviceClass;
import javax.bluetooth.DiscoveryAgent;
import javax.bluetooth.DiscoveryListener;
import javax.bluetooth.LocalDevice;
import javax.bluetooth.RemoteDevice;
import javax.bluetooth.ServiceRecord;

/**
 * Bluetoothデバイス探索サンプルプログラム
 */
public class BtSearch implements DiscoveryListener {
    /**
     * DiscoveryAgent
     */
    private DiscoveryAgent agent;
    
    /**
     * 初期化
     */
    public BtSearch() throws BluetoothStateException {
        LocalDevice local = LocalDevice.getLocalDevice();
        agent = local.getDiscoveryAgent();
    }



    /**
     * BtSearchメイン
     */
    public static void main(String[] args) {
        try {
            BtSearch client = new BtSearch();
            client.searchDevice();
        } catch (BluetoothStateException e) {
            e.printStackTrace();
        }
    }
    /**
     * Bluetoothデバイス検索メソッド
     */
    public void searchDevice() {
        try {
            agent.startInquiry(DiscoveryAgent.GIAC, this);

            synchronized (this) {
                try {
                    this.wait();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        } catch (BluetoothStateException e) {
            e.printStackTrace();
        }
    }

    /**
     * Bluetoothデバイス検出メソッド
     */
    public void deviceDiscovered(RemoteDevice btDevice, DeviceClass cod) {
        //デバイス情報の出力
        if(btDevice != null) {
            try {
                SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                System.out.println(df.format(new Date()) + " " +
                                                btDevice.getBluetoothAddress() + " " +
                                                btDevice.getFriendlyName(true));
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    /**
     * デバイス検索終了
     */
    public void inquiryCompleted(int discType) {
        synchronized (this) {
            try {
                this.notifyAll();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public void servicesDiscovered(int transID, ServiceRecord[] servRecord) {

    }

    public void serviceSearchCompleted(int transID, int respCode) {

    }

    
}
