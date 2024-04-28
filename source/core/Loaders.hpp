#pragma once

#include <PCH.hpp>
#include <nlohmann/json.hpp>

namespace brk {
	namespace meta {
		template <auto... Fields>
		struct FieldList
		{
			static constexpr size_t Count = sizeof...(Fields);
			static constexpr size_t TotalSize = (sizeof(decltype(Fields)) + ...);

			static_assert(
				(std::is_member_object_pointer_v<decltype(Fields)> && ...),
				"Fields must be pointers to member objects");

			const char* const m_Names[sizeof...(Fields)] = {};
		};
	} // namespace meta

	template <class T, class = void>
	struct BinaryLoader;

	template <class T, class = void>
	struct JsonLoader;

	namespace _internal {
		template <class T>
		struct IsFieldList : std::false_type
		{};

		template <auto... Members>
		struct IsFieldList<meta::FieldList<Members...>>
		{};
		template <auto... Members>
		struct IsFieldList<const meta::FieldList<Members...>> : std::true_type
		{};

		template <class T, class = void>
		struct IsNativeJsonCompatible : std::false_type
		{};
		template <class T>
		struct IsNativeJsonCompatible<
			T,
			std::void_t<
				std::enable_if_t<std::is_assignable_v<nlohmann::json, T>>,
				decltype(std::declval<const nlohmann::json>().get_to(
					std::declval<T&>()))>> : std::true_type
		{};

		template <class T>
		using IfHasFieldList = std::enable_if_t<IsFieldList<decltype(T::Fields)>::value>;
	} // namespace _internal

	template <class T>
	struct BinaryLoader<T, _internal::IfHasFieldList<T>>
	{
		static const uint8_t* Load(T& out_object, const uint8_t* data);

		static uint8_t* Save(const T& object, uint8_t* out_data);

	private:
		template <auto... Members>
		static const uint8_t* LoadImpl(
			const meta::FieldList<Members...>&,
			T& out_object,
			const uint8_t* data);

		template <auto... Fields>
		static uint8_t* SaveImpl(
			const meta::FieldList<Fields...>&,
			const T& obj,
			uint8_t* out_data);
	};

	template <class T>
	struct JsonLoader<T, _internal::IfHasFieldList<T>>
	{
		static T Load(const nlohmann::json& data);

		static nlohmann::json Save(const T& object);

	private:
		template <auto... Fields, size_t... I>
		static T LoadImpl(
			const meta::FieldList<Fields...>& list,
			std::index_sequence<I...>,
			const nlohmann::json& json);

		template <auto... Fields, size_t... I>
		static nlohmann::json SaveImpl(
			const meta::FieldList<Fields...>& list,
			std::index_sequence<I...>,
			const T& object);

		template <class F>
		static void LoadField(F& field, const nlohmann::json& json, const char* name);

		template <class F>
		static void SaveField(const F& field, nlohmann::json& out_json, const char* name);
	};
} // namespace brk

#include "Loaders.inl"