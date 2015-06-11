#ifndef UA_CLIENT_H_
#define UA_CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ua_util.h"
#include "ua_types.h"
#include "ua_connection.h"
#include "ua_log.h"
#include "ua_types_generated.h"

struct UA_Client;
typedef struct UA_Client UA_Client;

/**
 * The client networklayer is defined by a single function that fills a UA_Connection struct after
 * successfully connecting.
 */
typedef UA_Connection (*UA_ConnectClientConnection)(char *endpointUrl, UA_Logger *logger);

typedef struct UA_ClientConfig {
    UA_Int32 timeout; //sync response timeout
    UA_Int32 secureChannelLifeTime; // lifetime in ms (then the channel needs to be renewed)
    UA_Int32 timeToRenewSecureChannel; //time in ms  before expiration to renew the secure channel
    UA_ConnectionConfig localConnectionConfig;
} UA_ClientConfig;

extern UA_EXPORT const UA_ClientConfig UA_ClientConfig_standard;
UA_Client UA_EXPORT * UA_Client_new(UA_ClientConfig config, UA_Logger logger);

UA_EXPORT void UA_Client_delete(UA_Client* client);

UA_StatusCode UA_EXPORT UA_Client_connect(UA_Client *client, UA_ConnectClientConnection connFunc, char *endpointUrl);
UA_StatusCode UA_EXPORT UA_Client_disconnect(UA_Client *client);

UA_StatusCode UA_EXPORT UA_Client_renewSecureChannel(UA_Client *client);

/* Attribute Service Set */
UA_ReadResponse UA_EXPORT UA_Client_read(UA_Client *client, UA_ReadRequest *request);
UA_WriteResponse UA_EXPORT UA_Client_write(UA_Client *client, UA_WriteRequest *request);

/* View Service Set */    
UA_BrowseResponse UA_EXPORT UA_Client_browse(UA_Client *client, UA_BrowseRequest *request);
UA_BrowseNextResponse UA_EXPORT UA_Client_browseNext(UA_Client *client, UA_BrowseNextRequest *request);
UA_TranslateBrowsePathsToNodeIdsResponse UA_EXPORT
    UA_Client_translateTranslateBrowsePathsToNodeIds(UA_Client *client,
                                                     UA_TranslateBrowsePathsToNodeIdsRequest *request);

/* NodeManagement Service Set */
UA_AddNodesResponse UA_EXPORT UA_Client_addNodes(UA_Client *client, UA_AddNodesRequest *request);
UA_AddReferencesResponse UA_EXPORT
    UA_Client_addReferences(UA_Client *client, UA_AddReferencesRequest *request);

UA_DeleteNodesResponse UA_EXPORT UA_Client_deleteNodes(UA_Client *client, UA_DeleteNodesRequest *request);
UA_DeleteReferencesResponse UA_EXPORT
    UA_Client_deleteReferences(UA_Client *client, UA_DeleteReferencesRequest *request);
    
#ifdef __cplusplus
} // extern "C"
#endif


#ifdef ENABLE_SUBSCRIPTIONS

typedef struct UA_Client_MonitoredItem_s {
    UA_UInt32          ItemId;
    UA_UInt32          TimestampsToReturn;
    UA_UInt32          MonitoringMode;
    UA_NodeId          monitoredNodeId; 
    UA_UInt32          AttributeID;
    UA_UInt32          ClientHandle;
    UA_UInt32          SamplingInterval;
    UA_UInt32          QueueSize;
    UA_Boolean         DiscardOldest;
    LIST_ENTRY(UA_Client_MonitoredItem_s)  listEntry;
} UA_Client_MonitoredItem;

typedef struct UA_Client_Subscription_s {
    UA_UInt32    LifeTime;
    UA_Int32     KeepAliveCount;
    UA_DateTime  PublishingInterval;
    UA_Int32     SubscriptionID;
    UA_Int32     NotificationsPerPublish;
    UA_UInt32    Priority;
    LIST_ENTRY(UA_Client_Subscription_s) listEntry; //?
    LIST_HEAD(UA_ListOfUAMonitoredItems, UA_MonitoredItem_s) MonitoredItems;
} UA_Client_Subscription;

UA_CreateSubscriptionResponse UA_EXPORT UA_Client_createSubscription(UA_Client *client, UA_CreateSubscriptionRequest *request);
UA_DeleteSubscriptionsResponse UA_Client_deleteSubscriptions(UA_Client *client, UA_DeleteSubscriptionsRequest *request);

UA_Int32 UA_EXPORT UA_Client_newSubscription(UA_Client *client);
UA_StatusCode UA_EXPORT UA_Client_removeSubscription(UA_Client *client, UA_Int32 subscriptionId);

void UA_EXPORT UA_Client_modifySubscription(UA_Client *client);
void UA_EXPORT UA_Client_addMonitoredItem(UA_Client *client);
void UA_EXPORT UA_Client_publish(UA_Client *client);
void UA_EXPORT UA_Client_removeMonitoredItem(UA_Client *client);


#endif
#endif /* UA_CLIENT_H_ */
