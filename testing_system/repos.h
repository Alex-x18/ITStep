#pragma once
#include "testing_system.h"
namespace testing_system {
	interface cryptable {
		virtual void decrypt() = 0;
		virtual void encrypt() = 0;
	};
	class serialize_data;
	interface serializable {
		virtual serialize_data serialize() const = 0;
		virtual void deserialize(serialize_data& info) = 0;
	};
	template<class T, class CatType = std::string> interface categorizable {
		virtual std::set<CatType> fetch_cats() const noexcept = 0;
		virtual std::vector<T> fetch_by_cat(const CatType& cat) const noexcept = 0;
	};
	class serialize_data {
		public:
			serialize_data() = delete;
			serialize_data(const serialize_data&) = delete;
			serialize_data(serialize_data&& cp) noexcept:
				length(cp.length),
				data(std::move(cp.data)) {
				cp.length = 0;
			}
			serialize_data(size_t size):
				serialize_data(::operator new(size), size) {}
			serialize_data(void* data, size_t size):
				data(reinterpret_cast<char*>(data)),
				length(size) {}
			void assign(void* data, size_t size) noexcept {
				this->data.reset(reinterpret_cast<char*>(data));
				length = size;
			}
			void free() noexcept {
				data.reset();
				length = 0;
			}
			size_t size() noexcept {
				return length;
			}
			decltype(auto) get() noexcept {
				return data.get();
			}
			void operator=(const serialize_data&) = delete;
			serialize_data& operator=(serialize_data&& cp) noexcept {
				data = std::move(cp.data);
				length = cp.length;
				cp.length = 0;
			}
		private:
			std::unique_ptr<char[]> data;
			size_t length;
	};
	MAKE_EXCEPTION(fetch_exception);
	MAKE_EXCEPTION(deserialize_exception);
	MAKE_EXCEPTION(serialize_exception);
	MAKE_EXCEPTION(file_exception);
	template<class T> abstract class repos {
		public:
			repos(const std::string& filename):
				filename(filename) { }
			virtual void load() = 0;
			virtual void reload() = 0;
			virtual void save() = 0;
			void erase(const T& elem) noexcept {
				auto end = data.end();
				auto result_it = std::find_if(data.begin(), end, [&elem](const T& val) -> bool {
					return elem == val;
				});
				if (result_it != end) {
					data.erase(result_it);
				}
			}
			virtual void erase_and_update(const T& elem) {
				if (!loaded) {
					return;
				}
				size_t size_before = data.size();
				erase(elem);
				if (data.size() != size_before) {
					save();
				}
			}
			void insert(const T& elem) {
				data.push_back(elem);
			}
			virtual void insert_and_update(const T& elem) {
				if (!loaded) {
					return;
				}
				insert(elem);
				save();
			}
			T& fetch(const T& elem) const {
				return fetch([&elem](const T& val) -> bool {
					return elem == val;
				});
			}
			template<class Pred> T& fetch(Pred predicate) {
				auto end = data.end();
				auto res = std::find_if(data.begin(), end, predicate);
				if (res != end) {
					return *res;
				}
				throw fetch_exception("failed to fetch element");
			}
			template<class Pred> bool exists(Pred predicate) const noexcept {
				auto end = data.end();
				return std::find_if(data.begin(), end, predicate) != end;
			}
			virtual void release() noexcept {
				data.clear();
				loaded = false;
			}
			bool is_loaded() const noexcept {
				return loaded;
			}
			bool is_empty() const noexcept {
				return data.empty();
			}
			template<class Callback> void for_each(Callback callback) {
				for (auto& e : data) {
					callback(e);
				}
			}
			auto begin() const noexcept {
				return data.begin();
			}
			auto end() const noexcept {
				return data.end();
			}
			const std::string& get_filename() const noexcept {
				return filename;
			}
		protected:
			const std::string filename;
			bool loaded = false;
		private:
			std::vector<T> data;
	};
}