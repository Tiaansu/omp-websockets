//
// asio.hpp
// ~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_HPP
#define ASIO_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/append.hpp"
#include "asio/as_tuple.hpp"
#include "asio/associated_allocator.hpp"
#include "asio/associated_executor.hpp"
#include "asio/associated_cancellation_slot.hpp"
#include "asio/associator.hpp"
#include "asio/async_result.hpp"
#include "asio/awaitable.hpp"
#include "asio/basic_datagram_socket.hpp"
#include "asio/basic_deadline_timer.hpp"
#include "asio/basic_file.hpp"
#include "asio/basic_io_object.hpp"
#include "asio/basic_random_access_file.hpp"
#include "asio/basic_raw_socket.hpp"
#include "asio/basic_readable_pipe.hpp"
#include "asio/basic_seq_packet_socket.hpp"
#include "asio/basic_serial_port.hpp"
#include "asio/basic_signal_set.hpp"
#include "asio/basic_socket.hpp"
#include "asio/basic_socket_acceptor.hpp"
#include "asio/basic_socket_iostream.hpp"
#include "asio/basic_socket_streambuf.hpp"
#include "asio/basic_stream_file.hpp"
#include "asio/basic_stream_socket.hpp"
#include "asio/basic_streambuf.hpp"
#include "asio/basic_waitable_timer.hpp"
#include "asio/basic_writable_pipe.hpp"
#include "asio/bind_allocator.hpp"
#include "asio/bind_cancellation_slot.hpp"
#include "asio/bind_executor.hpp"
#include "asio/buffer.hpp"
#include "asio/buffer_registration.hpp"
#include "asio/buffered_read_stream_fwd.hpp"
#include "asio/buffered_read_stream.hpp"
#include "asio/buffered_stream_fwd.hpp"
#include "asio/buffered_stream.hpp"
#include "asio/buffered_write_stream_fwd.hpp"
#include "asio/buffered_write_stream.hpp"
#include "asio/buffers_iterator.hpp"
#include "asio/cancellation_signal.hpp"
#include "asio/cancellation_state.hpp"
#include "asio/cancellation_type.hpp"
#include "asio/co_spawn.hpp"
#include "asio/completion_condition.hpp"
#include "asio/compose.hpp"
#include "asio/connect.hpp"
#include "asio/connect_pipe.hpp"
#include "asio/coroutine.hpp"
#include "asio/deadline_timer.hpp"
#include "asio/defer.hpp"
#include "asio/deferred.hpp"
#include "asio/detached.hpp"
#include "asio/dispatch.hpp"
#include "asio/error.hpp"
#include "asio/error_code.hpp"
#include "asio/execution.hpp"
#include "asio/execution/allocator.hpp"
#include "asio/execution/any_executor.hpp"
#include "asio/execution/blocking.hpp"
#include "asio/execution/blocking_adaptation.hpp"
#include "asio/execution/bulk_execute.hpp"
#include "asio/execution/bulk_guarantee.hpp"
#include "asio/execution/connect.hpp"
#include "asio/execution/context.hpp"
#include "asio/execution/context_as.hpp"
#include "asio/execution/execute.hpp"
#include "asio/execution/executor.hpp"
#include "asio/execution/invocable_archetype.hpp"
#include "asio/execution/mapping.hpp"
#include "asio/execution/occupancy.hpp"
#include "asio/execution/operation_state.hpp"
#include "asio/execution/outstanding_work.hpp"
#include "asio/execution/prefer_only.hpp"
#include "asio/execution/receiver.hpp"
#include "asio/execution/receiver_invocation_error.hpp"
#include "asio/execution/relationship.hpp"
#include "asio/execution/schedule.hpp"
#include "asio/execution/scheduler.hpp"
#include "asio/execution/sender.hpp"
#include "asio/execution/set_done.hpp"
#include "asio/execution/set_error.hpp"
#include "asio/execution/set_value.hpp"
#include "asio/execution/start.hpp"
#include "asio/execution_context.hpp"
#include "asio/executor.hpp"
#include "asio/executor_work_guard.hpp"
#include "asio/file_base.hpp"
#include "asio/generic/basic_endpoint.hpp"
#include "asio/generic/datagram_protocol.hpp"
#include "asio/generic/raw_protocol.hpp"
#include "asio/generic/seq_packet_protocol.hpp"
#include "asio/generic/stream_protocol.hpp"
#include "asio/handler_alloc_hook.hpp"
#include "asio/handler_continuation_hook.hpp"
#include "asio/handler_invoke_hook.hpp"
#include "asio/high_resolution_timer.hpp"
#include "asio/io_context.hpp"
#include "asio/io_context_strand.hpp"
#include "asio/io_service.hpp"
#include "asio/io_service_strand.hpp"
#include "asio/ip/address.hpp"
#include "asio/ip/address_v4.hpp"
#include "asio/ip/address_v4_iterator.hpp"
#include "asio/ip/address_v4_range.hpp"
#include "asio/ip/address_v6.hpp"
#include "asio/ip/address_v6_iterator.hpp"
#include "asio/ip/address_v6_range.hpp"
#include "asio/ip/network_v4.hpp"
#include "asio/ip/network_v6.hpp"
#include "asio/ip/bad_address_cast.hpp"
#include "asio/ip/basic_endpoint.hpp"
#include "asio/ip/basic_resolver.hpp"
#include "asio/ip/basic_resolver_entry.hpp"
#include "asio/ip/basic_resolver_iterator.hpp"
#include "asio/ip/basic_resolver_query.hpp"
#include "asio/ip/host_name.hpp"
#include "asio/ip/icmp.hpp"
#include "asio/ip/multicast.hpp"
#include "asio/ip/resolver_base.hpp"
#include "asio/ip/resolver_query_base.hpp"
#include "asio/ip/tcp.hpp"
#include "asio/ip/udp.hpp"
#include "asio/ip/unicast.hpp"
#include "asio/ip/v6_only.hpp"
#include "asio/is_applicable_property.hpp"
#include "asio/is_contiguous_iterator.hpp"
#include "asio/is_executor.hpp"
#include "asio/is_read_buffered.hpp"
#include "asio/is_write_buffered.hpp"
#include "asio/local/basic_endpoint.hpp"
#include "asio/local/connect_pair.hpp"
#include "asio/local/datagram_protocol.hpp"
#include "asio/local/stream_protocol.hpp"
#include "asio/multiple_exceptions.hpp"
#include "asio/packaged_task.hpp"
#include "asio/placeholders.hpp"
#include "asio/posix/basic_descriptor.hpp"
#include "asio/posix/basic_stream_descriptor.hpp"
#include "asio/posix/descriptor.hpp"
#include "asio/posix/descriptor_base.hpp"
#include "asio/posix/stream_descriptor.hpp"
#include "asio/post.hpp"
#include "asio/prefer.hpp"
#include "asio/prepend.hpp"
#include "asio/query.hpp"
#include "asio/random_access_file.hpp"
#include "asio/read.hpp"
#include "asio/read_at.hpp"
#include "asio/read_until.hpp"
#include "asio/readable_pipe.hpp"
#include "asio/recycling_allocator.hpp"
#include "asio/redirect_error.hpp"
#include "asio/registered_buffer.hpp"
#include "asio/require.hpp"
#include "asio/require_concept.hpp"
#include "asio/serial_port.hpp"
#include "asio/serial_port_base.hpp"
#include "asio/signal_set.hpp"
#include "asio/socket_base.hpp"
#include "asio/static_thread_pool.hpp"
#include "asio/steady_timer.hpp"
#include "asio/strand.hpp"
#include "asio/stream_file.hpp"
#include "asio/streambuf.hpp"
#include "asio/system_context.hpp"
#include "asio/system_error.hpp"
#include "asio/system_executor.hpp"
#include "asio/system_timer.hpp"
#include "asio/this_coro.hpp"
#include "asio/thread.hpp"
#include "asio/thread_pool.hpp"
#include "asio/time_traits.hpp"
#include "asio/use_awaitable.hpp"
#include "asio/use_future.hpp"
#include "asio/uses_executor.hpp"
#include "asio/version.hpp"
#include "asio/wait_traits.hpp"
#include "asio/windows/basic_object_handle.hpp"
#include "asio/windows/basic_overlapped_handle.hpp"
#include "asio/windows/basic_random_access_handle.hpp"
#include "asio/windows/basic_stream_handle.hpp"
#include "asio/windows/object_handle.hpp"
#include "asio/windows/overlapped_handle.hpp"
#include "asio/windows/overlapped_ptr.hpp"
#include "asio/windows/random_access_handle.hpp"
#include "asio/windows/stream_handle.hpp"
#include "asio/writable_pipe.hpp"
#include "asio/write.hpp"
#include "asio/write_at.hpp"

#endif // ASIO_HPP