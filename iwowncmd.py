import gatt
import argparse


SERV_IWOWN = '0000ff20-0000-1000-8000-00805f9b34fb'
SERV_DFU = '00001530-1212-efde-1523-785feabcd123'

CHAR_WRITE = '0000ff21-0000-1000-8000-00805f9b34fb'
CHAR_NOTIFY = '0000ff22-0000-1000-8000-00805f9b34fb'
CHAR_INDICATE = '0000ff23-0000-1000-8000-00805f9b34fb'

CHAR_DFU_CONTROL = '00001531-1212-efde-1523-785feabcd123'
CHAR_DFU_DATA = '00001532-1212-efde-1523-785feabcd123'


class IWownDevice(gatt.Device):

    def set_command(self, cmd):
        self.cmd = cmd

    def services_resolved(self):
        super().services_resolved()
        self.dataacc = None

        self.dfu = None
        self.iwown = None
        for s in self.services:
            # print("SERVICE " + s.uuid)
            if s.uuid == SERV_IWOWN:
                self.iwown = s

            elif s.uuid == SERV_DFU:
                self.dfu = s

        if self.iwown:
            self.iwown_write = None
            self.iwown_indicate = None
            for c in self.iwown.characteristics:
                # print("CHARACTERISTIC " + c.uuid)
                if c.uuid == CHAR_WRITE:
                    self.iwown_write = c
                elif c.uuid == CHAR_INDICATE:
                    self.iwown_indicate = c
                    self.iwown_indicate.enable_notifications()

            self.iwown_write.write_value(cmd)

        if self.dfu:
            self.dfu_control = None
            for c in self.dfu.characteristics:
                print("CHARACTERISTIC " + c.uuid)
                if c.uuid == CHAR_DFU_CONTROL:
                    self.dfu_control = c

            # send system reset command
            self.dfu_control.write_value([6])

    @staticmethod
    def makeHeader(a, b):
        return ((a & 0xf) << 4) | (b & 0xf)

    def characteristic_value_updated(self, characteristic, value):
        # print("characteristic_value_updated {} {}".format(characteristic.uuid, value))

        if characteristic.uuid == CHAR_INDICATE:
            # accumulate data until we get desired size
            if self.dataacc is None:
                self.datatag = value[2]
                self.datalen = value[3]
                self.dataacc = value[4:]
            else:
                self.dataacc += value

            # if we've got all the data, process it!
            if self.datalen == len(self.dataacc):
                if self.datatag == 0:
                    fwversion = "{}.{}.{}.{}".format(self.dataacc[8], self.dataacc[9], self.dataacc[10], self.dataacc[11])
                    oadmode = (self.dataacc[6] << 8) | self.dataacc[7]
                    model = self.dataacc[2:6]
                    print("firmware: model={} version={} oadmode={}".format(model.decode('utf-8'), fwversion, oadmode))

                elif self.datatag == 1:
                    print("power {}%".format(self.dataacc[0]))

                else:
                    print("-----UNKNOWN RESULT----")
                    for i, x in enumerate(self.dataacc):
                        print("{}: {} {} {}".format(i, x, hex(x), chr(x)))

    def characteristic_read_value_succeeded(self, characteristic):
        print("characteristic_read_value_succeeded {}".format(characteristic.uuid))

    def characteristic_read_value_failed(self):
        print("characteristic_read_value_failed")

    def characteristic_write_value_succeeded(self, characteristic):
        # print("characteristic_write_value_succeeded {}".format(characteristic.uuid))
        pass

    def characteristic_write_value_failed(self):
        print("characteristic_write_value_failed")

    def characteristic_enable_notifications_succeeded(self, characteristic):
        pass
        # print("characteristic_enable_notifications_succeeded")

    def characteristic_enable_notifications_failed(self, characteristic, error):
        print("characteristic_enable_notifications_failed")


parser = argparse.ArgumentParser(description='IWOWN command tool')
parser.add_argument('--usbhost', default='hci0', help='USB host to use')
parser.add_argument('action', help='what to do!', choices=["getfwinfo", 'getpower', 'dfumode'])
parser.add_argument('mac', default='ff:ff:ff:ff:ff:ff', help='mac address')
args = parser.parse_args()

manager = gatt.DeviceManager(adapter_name=args.usbhost)
if args.action == 'getfwinfo':
    cmd = [0x21, 0xff, IWownDevice.makeHeader(0, 0), 0]
elif args.action == 'getpower':
    cmd = [0x21, 0xff, IWownDevice.makeHeader(0, 1), 0]
elif args.action == 'dfumode':
    cmd = [0x21, 0xff, IWownDevice.makeHeader(0, 3), 0]

device = IWownDevice(mac_address=args.mac, manager=manager)
device.set_command(cmd)
device.connect()
manager.run()
