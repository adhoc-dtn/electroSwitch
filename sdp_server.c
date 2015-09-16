#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/socket.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/rfcomm.h>


sdp_session_t *register_service();

int main(int argc, char **argv)
{
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    char str[1024] = { 0 };
    int s, client, bytes_read;
    sdp_session_t *session;
    socklen_t opt = sizeof(rem_addr);

    session = register_service();
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_L2CAP);
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t) 1;

    printf("Trying to bind...");
    fflush(stdout);
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
    printf("bound\n");
    fflush(stdout);
    printf("Waiting for connection...");
    fflush(stdout);
    listen(s, 1);
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);
    ba2str( &rem_addr.rc_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));
    bytes_read = read(client, buf, sizeof(buf));
    if( bytes_read > 0 ) {
     printf("received [%s]\n", buf);
    }
    sprintf(str,"to Android.");
    printf("sent [%s]\n",str);
    write(client, str, sizeof(str));
    close(client);
    close(s);
    sdp_close( session );

    return 0;
}

sdp_session_t *register_service()
{
  uint32_t svc_uuid_int[] = { 0x00000000,0x00000000,0x00000000,0x0001 };
  uint8_t rfcomm_channel = 1;
  const char *service_name = "Remote Host";
  const char *service_dsc = "What the remote should be connecting to.";
  const char *service_prov = "Your mother";

  uuid_t root_uuid, l2cap_uuid, rfcomm_uuid, svc_uuid;
  sdp_list_t *l2cap_list = 0,
       *rfcomm_list = 0,
       *root_list = 0,
       *proto_list = 0,
       *access_proto_list = 0;
  sdp_data_t *channel = 0, *psm = 0;

  sdp_record_t *record = sdp_record_alloc();

  // set the general service ID
  sdp_uuid128_create( &svc_uuid, &svc_uuid_int );
  sdp_set_service_id( record, svc_uuid );
  sdp_list_t service_class = {NULL, &svc_uuid};
  sdp_set_service_classes( record, &service_class);

  // make the service record publicly browsable
  sdp_uuid16_create(&root_uuid, PUBLIC_BROWSE_GROUP);
  root_list = sdp_list_append(0, &root_uuid);
  sdp_set_browse_groups( record, root_list );

  // set l2cap information
  sdp_uuid16_create(&l2cap_uuid, L2CAP_UUID);
  l2cap_list = sdp_list_append( 0, &l2cap_uuid );
  proto_list = sdp_list_append( 0, l2cap_list );

  // set rfcomm information
  sdp_uuid16_create(&rfcomm_uuid, RFCOMM_UUID);
  channel = sdp_data_alloc(SDP_UINT8, &rfcomm_channel);
  rfcomm_list = sdp_list_append( 0, &rfcomm_uuid );
  sdp_list_append( rfcomm_list, channel );
  sdp_list_append( proto_list, rfcomm_list );

  // attach protocol information to service record
  access_proto_list = sdp_list_append( 0, proto_list );
  sdp_set_access_protos( record, access_proto_list );

  // set the name, provider, and description
  sdp_set_info_attr(record, service_name, service_prov, service_dsc);

  int err = 0;
  sdp_session_t *session = 0;

  // connect to the local SDP server, register the service record, and
  // disconnect
  session = sdp_connect( BDADDR_ANY, BDADDR_LOCAL, SDP_RETRY_IF_BUSY );
  err = sdp_record_register(session, record, 0);

  // cleanup
  //sdp_data_free( channel );
  sdp_list_free( l2cap_list, 0 );
  sdp_list_free( rfcomm_list, 0 );
  sdp_list_free( root_list, 0 );
  sdp_list_free( access_proto_list, 0 );

  return session;
}
