#if !(defined(GO) && defined(GOM) && defined(GO2) && defined(DATA))
#error meh!
#endif
//GType is gulong !!!!!

//GO(__aeabi_f2lz, 
//GO(__aeabi_f2ulz, 
//GO(_fini, 
GO(g_array_get_type, LFv)
GO(g_binding_flags_get_type, LFv)
GO(g_binding_get_flags, uFp)
//GO(g_binding_get_source, 
//GO(g_binding_get_source_property, 
//GO(g_binding_get_target, 
GO(g_binding_get_target_property, pFp)
GO(g_binding_get_type, LFv)
GO(g_binding_unbind, vFp)
GO(g_boxed_copy, pFLp)
GO(g_boxed_free, vFLp)
GOM(g_boxed_type_register_static, LFEppp)
GO(g_byte_array_get_type, LFv)
GO(g_bytes_get_type, LFv)
DATA(g_cclosure_marshal_BOOLEAN__BOXED_BOXED, sizeof(void*))    // set all cclosure_marshal as data, so they can be used "as-is" in cclosure parameter (not-wrapped)
DATA(g_cclosure_marshal_BOOLEAN__BOXED_BOXEDv, sizeof(void*))
DATA(g_cclosure_marshal_BOOLEAN__FLAGS, sizeof(void*))
DATA(g_cclosure_marshal_BOOLEAN__FLAGSv, sizeof(void*))
DATA(g_cclosure_marshal_generic, sizeof(void*))
DATA(g_cclosure_marshal_generic_va, sizeof(void*))
DATA(g_cclosure_marshal_STRING__OBJECT_POINTER, sizeof(void*))
DATA(g_cclosure_marshal_STRING__OBJECT_POINTERv, sizeof(void*))
DATA(g_cclosure_marshal_VOID__BOOLEAN, sizeof(void*))
DATA(g_cclosure_marshal_VOID__BOOLEANv, sizeof(void*))
DATA(g_cclosure_marshal_VOID__BOXED, sizeof(void*))
DATA(g_cclosure_marshal_VOID__BOXEDv, sizeof(void*))
DATA(g_cclosure_marshal_VOID__CHAR, sizeof(void*))
DATA(g_cclosure_marshal_VOID__CHARv, sizeof(void*))
DATA(g_cclosure_marshal_VOID__DOUBLE, sizeof(void*))
DATA(g_cclosure_marshal_VOID__DOUBLEv, sizeof(void*))
DATA(g_cclosure_marshal_VOID__ENUM, sizeof(void*))
DATA(g_cclosure_marshal_VOID__ENUMv, sizeof(void*))
DATA(g_cclosure_marshal_VOID__FLAGS, sizeof(void*))
DATA(g_cclosure_marshal_VOID__FLAGSv, sizeof(void*))
DATA(g_cclosure_marshal_VOID__FLOAT, sizeof(void*))
DATA(g_cclosure_marshal_VOID__FLOATv, sizeof(void*))
DATA(g_cclosure_marshal_VOID__INT, sizeof(void*))
DATA(g_cclosure_marshal_VOID__INTv, sizeof(void*))
DATA(g_cclosure_marshal_VOID__LONG, sizeof(void*))
DATA(g_cclosure_marshal_VOID__LONGv, sizeof(void*))
DATA(g_cclosure_marshal_VOID__OBJECT, sizeof(void*))
DATA(g_cclosure_marshal_VOID__OBJECTv, sizeof(void*))
DATA(g_cclosure_marshal_VOID__PARAM, sizeof(void*))
DATA(g_cclosure_marshal_VOID__PARAMv, sizeof(void*))
DATA(g_cclosure_marshal_VOID__POINTER, sizeof(void*))
DATA(g_cclosure_marshal_VOID__POINTERv, sizeof(void*))
DATA(g_cclosure_marshal_VOID__STRING, sizeof(void*))
DATA(g_cclosure_marshal_VOID__STRINGv, sizeof(void*))
DATA(g_cclosure_marshal_VOID__UCHAR, sizeof(void*))
DATA(g_cclosure_marshal_VOID__UCHARv, sizeof(void*))
DATA(g_cclosure_marshal_VOID__UINT, sizeof(void*))
DATA(g_cclosure_marshal_VOID__UINT_POINTER, sizeof(void*))
DATA(g_cclosure_marshal_VOID__UINT_POINTERv, sizeof(void*))
DATA(g_cclosure_marshal_VOID__UINTv, sizeof(void*))
DATA(g_cclosure_marshal_VOID__ULONG, sizeof(void*))
DATA(g_cclosure_marshal_VOID__ULONGv, sizeof(void*))
DATA(g_cclosure_marshal_VOID__VARIANT, sizeof(void*))
DATA(g_cclosure_marshal_VOID__VARIANTv, sizeof(void*))
DATA(g_cclosure_marshal_VOID__VOID, sizeof(void*))
DATA(g_cclosure_marshal_VOID__VOIDv, sizeof(void*))
//GO(g_cclosure_new, 
//GO(g_cclosure_new_object, 
//GO(g_cclosure_new_object_swap, 
//GO(g_cclosure_new_swap, 
//GO(g_checksum_get_type, 
GO(g_clear_object, vFp)
GOM(g_closure_add_finalize_notifier, vFEppp)
//GO(g_closure_add_invalidate_notifier, 
//GO(g_closure_add_marshal_guards, 
//GO(g_closure_get_type, 
//GO(g_closure_invalidate, 
//GO(g_closure_invoke, 
//GO(g_closure_new_object, 
GO(g_closure_new_simple, pFup)
//GO(g_closure_ref, 
//GO(g_closure_remove_finalize_notifier, 
//GO(g_closure_remove_invalidate_notifier, 
GOM(g_closure_set_marshal, vFEpp)
//GO(g_closure_set_meta_marshal, 
//GO(g_closure_sink, 
//GO(g_closure_unref, 
GO(g_date_get_type, LFv)
GO(g_date_time_get_type, LFv)
GO(g_enum_complete_type_info, vFLpp)
GO(g_enum_get_value, pFpi)
GO(g_enum_get_value_by_name, pFpp)
GO(g_enum_get_value_by_nick, pFpp)
GO(g_enum_register_static, LFpp)
GO(g_error_get_type, LFv)
GO(g_flags_complete_type_info, vFipp)
GO(g_flags_get_first_value, pFpu)
GO(g_flags_get_value_by_name, pFpp)
GO(g_flags_get_value_by_nick, pFpp)
GO(g_flags_register_static, LFpp)
GO(g_gstring_get_type, LFv)
GO(g_gtype_get_type, LFv)
GO(g_hash_table_get_type, LFv)
GO(g_initially_unowned_get_type, LFv)
GO(g_io_channel_get_type, LFv)
GO(g_io_condition_get_type, LFv)
GO(g_key_file_get_type, LFv)
GO(g_main_context_get_type, LFv)
GO(g_main_loop_get_type, LFv)
GO(g_mapped_file_get_type, LFv)
GO(g_markup_parse_context_get_type, LFv)
GO(g_match_info_get_type, LFv)
//GOM(g_object_add_toggle_ref, vFEpBp)
GO(g_object_add_weak_pointer, vFpp)
GO(g_object_bind_property, pFppppu)
//GO(g_object_bind_property_full, 
//GO(g_object_bind_property_with_closures, 
GO(g_object_class_find_property, pFpp)
GOM(g_object_class_install_properties, vFEpup)
GO(g_object_class_install_property, vFpup)
GO(g_object_class_list_properties, pFpp)
GO(g_object_class_override_property, vFpup)
GO(g_object_compat_control, LFLp)
GOM(g_object_connect, pFEppV)
GO(g_object_disconnect, vFpppppppppppppppp)   // caarg
//GOM(g_object_dup_data, pFEppBp)
//GOM(g_object_dup_qdata, pFEppBp)
GO(g_object_force_floating, vFp)
GO(g_object_freeze_notify, vFp)
GOM(g_object_get, vFEppV)
GO(g_object_get_data, pFpp)
GO(g_object_get_property, vFppp)
GO(g_object_get_qdata, pFpp)
GO(g_object_get_type, LFv)
GOM(g_object_get_valist, vFEppA)
GO(g_object_interface_find_property, pFpp)
GO(g_object_interface_install_property, vFpp)
GO(g_object_interface_list_properties, pFpp)
GO(g_object_is_floating, iFp)
GOM(g_object_new, pFELpV)
GO(g_object_newv, pFLup)
GOM(g_object_new_valist, pFELpA)
GO(g_object_notify, vFpp)
GO(g_object_notify_by_pspec, vFpp)
GO(g_object_ref, pFp)
GO(g_object_ref_sink, pFp)
//GOM(g_object_remove_toggle_ref, vFEpBp)
GO(g_object_remove_weak_pointer, vFpp)
//GOM(g_object_replace_data, iFEppppBB)
//GOM(g_object_replace_qdata, iFEppppBB)
GO(g_object_run_dispose, vFp)
GOM(g_object_set, vFEppV)
GO(g_object_set_data, vFppp)
GOM(g_object_set_data_full, vFEpppp)
GO(g_object_set_property, vFppp)
GO(g_object_set_qdata, vFpup)
GOM(g_object_set_qdata_full, vFEpppp)
GOM(g_object_set_valist, vFEppA)
GO(g_object_steal_data, pFpp)
GO(g_object_steal_qdata, pFpu)
GO(g_object_thaw_notify, vFp)
GO(g_object_unref, vFp)
GO(g_object_watch_closure, vFpp)
GOM(g_object_weak_ref, vFEppp)
GO(g_object_weak_unref, vFpp)
GO(g_param_spec_boolean, pFpppii)
GO(g_param_spec_boxed, pFpppLi)
GO(g_param_spec_char, pFpppccci)
GO(g_param_spec_double, pFpppdddi)
GO(g_param_spec_enum, pFpppLii)
GO(g_param_spec_flags, pFpppLui)
GO(g_param_spec_float, pFpppfffi)
GO(g_param_spec_get_blurb, pFp)
GO(g_param_spec_get_default_value, pFp)
GO(g_param_spec_get_name, pFp)
GO(g_param_spec_get_nick, pFp)
GO(g_param_spec_get_qdata, pFpu)
GO(g_param_spec_get_redirect_target, pFp)
GO(g_param_spec_gtype, pFpppLi)
GO(g_param_spec_int, pFpppiiii)
GO(g_param_spec_int64, pFpppllli)
GO(g_param_spec_internal, pFLpppi)
GO(g_param_spec_long, pFpppllli)
GO(g_param_spec_object, pFpppLi)
GO(g_param_spec_override, pFpp)
GO(g_param_spec_param, pFpppii)
GO(g_param_spec_pointer, pFpppi)
GO(g_param_spec_pool_insert, vFppL)
GO(g_param_spec_pool_list, pFpLp)
GO(g_param_spec_pool_list_owned, pFpL)
GO(g_param_spec_pool_lookup, pFppLi)
GO(g_param_spec_pool_new, pFi)
GO(g_param_spec_pool_remove, vFpp)
GO(g_param_spec_ref, pFp)
GO(g_param_spec_ref_sink, pFp)
GO(g_param_spec_set_qdata, vFpup)
GOM(g_param_spec_set_qdata_full, vFEpupp)
GO(g_param_spec_sink, vFp)
GO(g_param_spec_steal_qdata, pFpu)
GO(g_param_spec_string, pFppppi)
DATA(g_param_spec_types, sizeof(void*))
GO(g_param_spec_uchar, pFpppCCCi)
GO(g_param_spec_uint, pFpppuuui)
GO(g_param_spec_uint64, pFpppLLLi)
GO(g_param_spec_ulong, pFpppLLLi)
GO(g_param_spec_unichar, pFpppui)
GO(g_param_spec_unref, vFp)
GO(g_param_spec_value_array, pFppppi)
GO(g_param_spec_variant, pFpppppi)
GOM(g_param_type_register_static, iFEpp)
GO(g_param_value_convert, iFpppi)
GO(g_param_value_defaults, iFpp)
GO(g_param_values_cmp, iFppp)
GO(g_param_value_set_default, vFpp)
GO(g_param_value_validate, iFpp)
GO(g_pointer_type_register_static, LFp)
//GO(g_pollfd_get_type, 
//GO(g_ptr_array_get_type, 
//GO(g_regex_get_type, 
GO(g_signal_accumulator_first_wins, iFpppp)
GO(g_signal_accumulator_true_handled, iFpppp)
GOM(g_signal_add_emission_hook, LFEupppp)
GO(g_signal_chain_from_overridden, vFpp)
//GO(g_signal_chain_from_overridden_handler, vFpppppppppp)  //vaarg
GO(g_signal_connect_closure, LFpppi)
GO(g_signal_connect_closure_by_id, LFpuppi)
GOM(g_signal_connect_data, LFEpppppu)
GOM(g_signal_connect_object, LFEppppi)
GOM(g_signal_emit, vFEpuuV)
GO(g_signal_emit_by_name, vFppppppppppp)    //vaarg
GO(g_signal_emitv, vFpuup)
GOM(g_signal_emit_valist, vFEpupA)    // va_list here
GO(g_signal_get_invocation_hint, pFp)
GO(g_signal_handler_block, vFpL)
GO(g_signal_handler_disconnect, vFpL)
GOM(g_signal_handler_find, LFEpiupppp)
GO(g_signal_handler_is_connected, iFpL)
GOM(g_signal_handlers_block_matched, uFEpiupppp)
//GO(g_signal_handlers_destroy, 
GOM(g_signal_handlers_disconnect_matched, uFEpiupppp)
GOM(g_signal_handlers_unblock_matched, uFEpiupppp)
GO(g_signal_handler_unblock, vFpL)
GO(g_signal_has_handler_pending, iFpuui)
GO(g_signal_list_ids, pFLp)
GO(g_signal_lookup, uFpL)
GO(g_signal_name, pFu)
GOM(g_signal_new, uFEpLiupppLuV)
//GOM(g_signal_new_class_handler, uFEpLupppppnV)
GOM(g_signal_newv, uFEpLippppLup)
GOM(g_signal_new_valist, uFEpLippppLuA)
GO(g_signal_override_class_closure, vFuLp)
GOM(g_signal_override_class_handler, vFEppp)
GO(g_signal_parse_name, iFpLppi)
GO(g_signal_query, vFup)
GO(g_signal_remove_emission_hook, vFuL)
//GOM(g_signal_set_va_marshaller, vFEuiB)
GO(g_signal_stop_emission, vFpuu)
GO(g_signal_stop_emission_by_name, vFpp)
GO(g_signal_type_cclosure_new, pFLu)    //should wrap?
GO(g_source_get_type, LFv)
GO(g_source_set_closure, vFpp)
//GO(g_source_set_dummy_callback, 
GO(g_strdup_value_contents, pFp)
GO(g_strv_get_type, LFv)
GO(g_thread_get_type, LFv)
GO(g_time_zone_get_type, LFv)
//GOM(g_type_add_class_cache_func, vFEpB)
GO(g_type_add_class_private, vFLL)
GO(g_type_add_instance_private, iFLL)
//GOM(g_type_add_interface_check, vFEpB)
//GOM(g_type_add_interface_dynamic, vFEiip)
GOM(g_type_add_interface_static, vFELLp)
GO(g_type_check_class_cast, pFpL)
GO(g_type_check_class_is_a, iFpL)
GO(g_type_check_instance, iFp)
GO(g_type_check_instance_cast, pFpL)
GO(g_type_check_instance_is_a, LFpL)
GO(g_type_check_instance_is_fundamentally_a, iFpL)
GO(g_type_check_is_value_type, iFL)
GO(g_type_check_value, LFp)
GO(g_type_check_value_holds, iFpL)
GO(g_type_children, pFip)
GO(g_type_class_add_private, vFpL)
GO(g_type_class_adjust_private_offset, vFpp)
GO(g_type_class_get_instance_private_offset, iFp)
GO(g_type_class_get_private, pFpL)
GO(g_type_class_peek, pFi)
GOM(g_type_class_peek_parent, pFEp)
GO(g_type_class_peek_static, pFi)
GO(g_type_class_ref, pFp)
GO(g_type_class_unref, vFp)
GO(g_type_class_unref_uncached, vFp)
GO(g_type_create_instance, pFL)
GO(g_type_default_interface_peek, pFL)
GO(g_type_default_interface_ref, pFi)
GO(g_type_default_interface_unref, vFp)
GO(g_type_depth, uFL)
GO(g_type_ensure, vFi)
GO(g_type_free_instance, vFp)
GO(g_type_from_name, iFp)
GO(g_type_fundamental, LFL)
GO(g_type_fundamental_next, iFv)
//GOM(g_type_get_plugin, pFEi)      // GTypePugin is a stuct with callback
GO(g_type_get_qdata, pFLu)
GO(g_type_get_type_registration_serial, uFv)
GO(g_type_init, vFv)
GO(g_type_init_with_debug_flags, vFu)
GO(g_type_instance_get_private, pFpL)
GO(g_type_interface_add_prerequisite, vFLL)
//GOM(g_type_interface_get_plugin, pFEii) // return GTypePlugin*
GO(g_type_interface_peek, pFpL)
GO(g_type_interface_peek_parent, pFp)
GO(g_type_interface_prerequisites, pFLp)
GO(g_type_interfaces, pFLp)
GO(g_type_is_a, iFLL)
//GO(g_type_module_add_interface, 
//GO(g_type_module_get_type, 
GO(g_type_module_register_enum, LFppp)
GO(g_type_module_register_flags, LFppp)
//GO(g_type_module_register_type, 
GO(g_type_module_set_name, vFpp)
GO(g_type_module_unuse, vFp)
GO(g_type_module_use, iFp)
GO(g_type_name, pFL)
GO(g_type_name_from_class, pFp)
GO(g_type_name_from_instance, pFp)
GO(g_type_next_base, LFLL)
GO(g_type_parent, LFL)
//GO(g_type_plugin_complete_interface_info, 
//GO(g_type_plugin_complete_type_info, 
//GO(g_type_plugin_get_type, 
GO(g_type_plugin_unuse, vFp)
//GO(g_type_plugin_use, 
GO(g_type_qname, uFL)
GO(g_type_query, vFLp)
//GOM(type_register_dynamic, iFEpippi)
GOM(g_type_register_fundamental, LFELpppi)
GOM(g_type_register_static, LFELppi)
GOM(g_type_register_static_simple, LFELpupupu)
//GOM(g_type_remove_class_cache_func, vFEpB)
//GOM(g_type_remove_interface_check, vFEpB)
GO(g_type_set_qdata, vFLup)
GO(g_type_test_flags, iFLu)
GOM(g_type_value_table_peek, pFEL)    //need to bridge GTypeValueTable
GO(g_value_array_append, pFpp)  //TODO: does GValueArray needs alignement?
GO(g_value_array_copy, pFp)
GO(g_value_array_free, vFp)
GO(g_value_array_get_nth, pFpu)
GO(g_value_array_get_type, LFv)
GO(g_value_array_insert, pFpup)
GO(g_value_array_new, pFu)
GO(g_value_array_prepend, pFpp)
GO(g_value_array_remove, pFpu)
GOM(g_value_array_sort, pFEpp)  // TODO: need to use a CompareFunct with GValue alignment work
GOM(g_value_array_sort_with_data, pFEppp)   //TODO: same here
GO(g_value_copy, vFpp)
GO(g_value_dup_boxed, pFp)
GO(g_value_dup_object, pFp)
GO(g_value_dup_param, pFp)
GO(g_value_dup_string, pFp)
GO(g_value_dup_variant, pFp)
GO(g_value_fits_pointer, iFp)
GO(g_value_get_boolean, iFp)
GO(g_value_get_boxed, pFp)
GO(g_value_get_char, CFp)
GO(g_value_get_double, dFp)
GO(g_value_get_enum, iFp)
GO(g_value_get_flags, uFp)
GO(g_value_get_float, fFp)
GO(g_value_get_gtype, LFp)
GO(g_value_get_int, iFp)
GO(g_value_get_int64, lFp)
GO(g_value_get_long, lFp)
GO(g_value_get_object, pFp)
GO(g_value_get_param, pFp)
GO(g_value_get_pointer, pFp)
GO(g_value_get_schar, cFp)
GO(g_value_get_string, pFp)
GO(g_value_get_type, LFv)
GO(g_value_get_uchar, CFp)
GO(g_value_get_uint, uFp)
GO(g_value_get_uint64, LFp)
GO(g_value_get_ulong, LFp)
GO(g_value_get_variant, pFp)
GO(g_value_init, pFpL)
GO(g_value_init_from_instance, vFpp)
GO(g_value_peek_pointer, pFp)
GOM(g_value_register_transform_func, vFEiip)
GO(g_value_reset, pFp)
GO(g_value_set_boolean, vFpi)
GO(g_value_set_boxed, vFpp)
GO(g_value_set_boxed_take_ownership, vFpp)
GO(g_value_set_char, vFpC)
GO(g_value_set_double, vFpd)
GO(g_value_set_enum, vFpi)
GO(g_value_set_flags, vFpu)
GO(g_value_set_float, vFpf)
GO(g_value_set_gtype, vFpL)
GO(g_value_set_instance, vFpp)
GO(g_value_set_int, vFpi)
GO(g_value_set_int64, vFpl)
GO(g_value_set_long, vFpl)
GO(g_value_set_object, vFpp)
GO(g_value_set_object_take_ownership, vFpp)
GO(g_value_set_param, vFpp)
GO(g_value_set_param_take_ownership, vFpp)
GO(g_value_set_pointer, vFpp)
GO(g_value_set_schar, vFpc)
GO(g_value_set_static_boxed, vFpp)
GO(g_value_set_static_string, vFpp)
GO(g_value_set_string, vFpp)
GO(g_value_set_string_take_ownership, vFpp)
GO(g_value_set_uchar, vFpC)
GO(g_value_set_uint, vFpu)
GO(g_value_set_uint64, vFpL)
GO(g_value_set_ulong, vFpL)
GO(g_value_set_variant, vFpp)
GO(g_value_take_boxed, vFpp)
GO(g_value_take_object, vFpp)
GO(g_value_take_param, vFpp)
GO(g_value_take_string, vFpp)
GO(g_value_take_variant, vFpp)
GO(g_value_transform, iFpp)
GO(g_value_type_compatible, iFii)
GO(g_value_type_transformable, iFLL)
GO(g_value_unset, vFp)
//GO(g_variant_builder_get_type, 
//GO(g_variant_dict_get_type, 
//GO(g_variant_get_gtype, 
//GO(g_variant_type_get_gtype, 
GO(g_weak_ref_clear, vFp)
GO(g_weak_ref_get, pFp)
GO(g_weak_ref_init, vFpp)
GO(g_weak_ref_set, vFpp)
//GO(_init, 

// to have some wrapper created for gtkclass.c
GO(dummy_set_property, vFpupp)  
GO(dummy_drag_data_get, vFpppuu)
GO(dummy_drag_motion, iFppiiu)
GO(dummy_drag_data_received, vFppiipuu)
GO(dummy_childprop, vFppupp)
GO(dummy_marshal, vFppuppp)
GO(dummy_marshalv, vFpppppip)
