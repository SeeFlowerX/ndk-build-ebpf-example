#include <linux/bpf.h>
#include <linux/ptrace.h>
#include <bpf_helpers.h>

#define TASK_COMM_LEN 16

#define __uint(name, val) int (*name)[val]
#define __type(name, val) typeof(val) *name
#define __array(name, val) typeof(val) *name[]

struct pt_regs {
	union {
		struct user_pt_regs user_regs;
		struct {
			uint64_t regs[31];
			uint64_t sp;
			uint64_t pc;
			uint64_t pstate;
		};
	};
	uint64_t orig_x0;
	signed int syscallno;
	uint32_t unused2;
	uint64_t orig_addr_limit;
	uint64_t pmr_save;
	uint64_t stackframe[2];
	uint64_t lockdep_hardirqs;
	uint64_t exit_rcu;
};

struct hook_data_event_t {
    uint32_t pid;
    uint32_t tid;
    uint64_t timestamp_ns;
    char comm[TASK_COMM_LEN];
};

struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
} stack_events SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_PERCPU_ARRAY);
    __type(key, uint32_t);
    __type(value, struct hook_data_event_t);
    __uint(max_entries, 1);
} data_buffer_heap SEC(".maps");

SEC("uprobe/stack")
int probe_stack(struct pt_regs* ctx) {
    uint64_t current_pid_tgid = bpf_get_current_pid_tgid();
    uint32_t pid = current_pid_tgid >> 32;
    uint32_t tid = current_pid_tgid & 0xffffffff;
    uint64_t current_uid_gid = bpf_get_current_uid_gid();
    uint32_t uid = current_uid_gid >> 32;

    if (0xaabbccaa != uid) {
        return 0;
    }

    uint32_t zero = 0;
    struct hook_data_event_t* event = bpf_map_lookup_elem_unsafe(&data_buffer_heap, &zero);
    if (event == NULL) {
        return 0;
    }

    event->pid = pid;
    event->tid = tid;
    event->timestamp_ns = bpf_ktime_get_ns();

    bpf_get_current_comm(&event->comm, sizeof(event->comm));

    bpf_perf_event_output(ctx, &stack_events, 0, event, sizeof(struct hook_data_event_t));
    return 0;
}

char __license[] SEC("license") = "GPL";
__u32 _version SEC("version") = 0xFFFFFFFE;