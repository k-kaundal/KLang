#ifndef CLOUD_NATIVE_H
#define CLOUD_NATIVE_H

#include <stdio.h>
#include "interpreter.h"

/* Container runtime support */
typedef struct {
    char *image;
    char *name;
    char **env_vars;
    int env_count;
    char **volumes;
    int volume_count;
    int memory_limit_mb;
    int cpu_limit;
} ContainerSpec;

typedef struct {
    char *id;
    char *status;
    ContainerSpec *spec;
} Container;

Container* container_create(ContainerSpec *spec);
int container_start(Container *container);
int container_stop(Container *container);
int container_restart(Container *container);
void container_destroy(Container *container);
char* container_logs(Container *container);

/* Kubernetes integration */
typedef struct {
    char *name;
    char *namespace;
    int replicas;
    char *image;
    char **labels;
    int label_count;
} K8sDeployment;

typedef struct {
    char *name;
    char *namespace;
    char *type;  /* ClusterIP, NodePort, LoadBalancer */
    int port;
    int target_port;
} K8sService;

typedef struct {
    char *cluster_url;
    char *token;
    char *namespace;
} K8sClient;

K8sClient* k8s_client_create(const char *cluster_url, const char *token);
void k8s_client_destroy(K8sClient *client);

int k8s_deploy(K8sClient *client, K8sDeployment *deployment);
int k8s_create_service(K8sClient *client, K8sService *service);
int k8s_scale_deployment(K8sClient *client, const char *name, int replicas);
char* k8s_get_pods(K8sClient *client, const char *deployment);
char* k8s_get_logs(K8sClient *client, const char *pod_name);

/* Serverless/FaaS support */
typedef struct {
    char *name;
    char *runtime;  /* "klang", "nodejs", etc. */
    char *handler;  /* Function to call */
    char *code;     /* Function code */
    int memory_mb;
    int timeout_seconds;
    char **env_vars;
    int env_count;
} FunctionSpec;

typedef struct {
    char *id;
    char *name;
    char *status;
    FunctionSpec *spec;
} CloudFunction;

CloudFunction* function_create(FunctionSpec *spec);
Value function_invoke(CloudFunction *func, Value *args, int arg_count);
void function_destroy(CloudFunction *func);

/* Service mesh integration */
typedef struct {
    char *service_name;
    char *version;
    char **endpoints;
    int endpoint_count;
} ServiceRegistration;

typedef struct {
    char *name;
    char *registry_url;
} ServiceMesh;

ServiceMesh* service_mesh_create(const char *name, const char *registry_url);
void service_mesh_destroy(ServiceMesh *mesh);

int service_mesh_register(ServiceMesh *mesh, ServiceRegistration *registration);
int service_mesh_deregister(ServiceMesh *mesh, const char *service_name);
ServiceRegistration* service_mesh_discover(ServiceMesh *mesh, const char *service_name);
char** service_mesh_list_services(ServiceMesh *mesh, int *count);

/* Distributed tracing */
typedef struct {
    char *trace_id;
    char *span_id;
    char *parent_span_id;
    char *operation_name;
    long start_time;
    long duration;
    char **tags;
    int tag_count;
} Span;

typedef struct {
    char *trace_id;
    Span **spans;
    int span_count;
} Trace;

typedef struct {
    char *collector_url;
    char *service_name;
} TracingContext;

TracingContext* tracing_init(const char *collector_url, const char *service_name);
void tracing_shutdown(TracingContext *ctx);

Span* span_create(TracingContext *ctx, const char *operation_name);
void span_set_tag(Span *span, const char *key, const char *value);
void span_finish(Span *span);
void span_destroy(Span *span);

/* Metrics and monitoring */
typedef enum {
    METRIC_COUNTER,
    METRIC_GAUGE,
    METRIC_HISTOGRAM
} MetricType;

typedef struct {
    char *name;
    MetricType type;
    double value;
    char **labels;
    int label_count;
} Metric;

typedef struct {
    char *push_gateway_url;
    char *job_name;
    Metric **metrics;
    int metric_count;
} MetricsCollector;

MetricsCollector* metrics_init(const char *push_gateway_url, const char *job_name);
void metrics_shutdown(MetricsCollector *collector);

void metrics_counter_inc(MetricsCollector *collector, const char *name, double value);
void metrics_gauge_set(MetricsCollector *collector, const char *name, double value);
void metrics_histogram_observe(MetricsCollector *collector, const char *name, double value);
int metrics_push(MetricsCollector *collector);

/* Health checks */
typedef enum {
    HEALTH_UNKNOWN,
    HEALTH_HEALTHY,
    HEALTH_UNHEALTHY,
    HEALTH_DEGRADED
} HealthStatus;

typedef struct {
    char *component;
    HealthStatus status;
    char *message;
} HealthCheck;

typedef struct {
    HealthCheck **checks;
    int check_count;
    HealthStatus overall_status;
} HealthReport;

HealthReport* health_report_create(void);
void health_report_add_check(HealthReport *report, const char *component, HealthStatus status, const char *message);
HealthStatus health_report_get_overall(HealthReport *report);
char* health_report_to_json(HealthReport *report);
void health_report_destroy(HealthReport *report);

/* Configuration management */
typedef struct {
    char *key;
    char *value;
} ConfigEntry;

typedef struct {
    ConfigEntry **entries;
    int count;
    char *config_server_url;
} ConfigManager;

ConfigManager* config_manager_create(const char *config_server_url);
void config_manager_destroy(ConfigManager *manager);

int config_manager_load(ConfigManager *manager);
char* config_manager_get(ConfigManager *manager, const char *key);
int config_manager_set(ConfigManager *manager, const char *key, const char *value);
int config_manager_sync(ConfigManager *manager);

/* Cloud deployment helpers */
int cloud_deploy_to_aws(const char *function_code, const char *function_name);
int cloud_deploy_to_azure(const char *function_code, const char *function_name);
int cloud_deploy_to_gcp(const char *function_code, const char *function_name);

#endif /* CLOUD_NATIVE_H */
