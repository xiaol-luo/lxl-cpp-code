#pragma once

#include <functional>
#include <unordered_map>
#include <assert.h>

class SubcribeDataBase 
{
public:
	const static uint32_t INVALID_ID = 0;
public:
	SubcribeDataBase() {}
	virtual ~SubcribeDataBase() { this->Release(); }
	virtual void Release() {};
	virtual void Remove(uint32_t id) = 0;
	virtual size_t Count() = 0;
	uint32_t next_id = 0;

	std::unordered_map<uint32_t, void*> fs_ptr;
	template <typename... Params>
	void ForeachFs(Params... params)
	{
		for (auto &&kv_pair : fs_ptr)
		{
			auto fn = (std::function<void(Params...)> *)(kv_pair.second);
			(*fn)(params...);
		}
	}
};

template <typename...Args>
class SubscribeData : public SubcribeDataBase
{
public:
	using stl_function_type = std::function<void(Args...)>;
	std::unordered_map<uint32_t, stl_function_type> fs;

	uint32_t Add(stl_function_type f)
	{
		++next_id;
		(INVALID_ID == next_id) ? next_id = 1 : 0;
		bool ret = false;
		std::unordered_map<uint32_t, stl_function_type>::iterator it;
		std::tie(it, ret) = fs.insert(std::make_pair(next_id, f));
		if (ret) fs_ptr.insert(std::make_pair(next_id, (void*)&it->second));
		return ret ? next_id : INVALID_ID;
	}
	virtual void Remove(uint32_t id) override
	{
		fs.erase(id);
		fs_ptr.erase(id);
	}
	virtual size_t Count() override
	{
		return fs.size();
	}
	virtual void Release() override
	{
		fs.clear();
		fs_ptr.clear();
	}
};

class EventDispacher
{
public:
	static const uint64_t INVALID_ID = 0;
	union SubscribeId
	{
		struct 
		{
			int event_id;
			int f_key;
		};
		uint64_t id;
	};

public:
	EventDispacher();
	~EventDispacher();

	template <typename... Args>
	void Fire(int id, Args... args)
	{
		auto it = m_subscribes.find(id);
		if (m_subscribes.end() == it)
			return;
		it->second->ForeachFs(args...);
	}

	template <typename return_type, typename... Args>
	int64_t subscribe(int id, std::function<return_type(Args...)>& f)
	{
		auto it = m_subscribes.find(id);
		if (m_subscribes.end() == it)
		{
			bool ret = false;
			std::tie(it, ret) = m_subscribes.insert(std::make_pair(id, new SubscribeData<Args...>()));
			if (!ret) return INVALID_ID;
		}
		SubscribeData<Args...> *subscribe = dynamic_cast<SubscribeData<Args...> *>(it->second);
		assert(subscribe);
		uint32_t f_key = subscribe->Add(f);
		if (SubcribeDataBase::INVALID_ID == f_key)
			return INVALID_ID;

		SubscribeId sid;
		sid.event_id = id;
		sid.f_key = f_key;
		return sid.id;
	}

	template <typename return_type, typename... Args>
	int64_t subscribe(int id, return_type(*f)(Args...))
	{
		std::function<void(Args...)> fn = f;
		return subscribe(id, fn);
	}

	template <typename... Args, typename binder>
	int64_t subscribe(int id, binder b)
	{
		std::function<void(Args...)> fn = b;
		return subscribe(id, fn);
	}

	void Cancel(int64_t subscribe_id)
	{
		SubscribeId sid;
		sid.id = subscribe_id;

		auto it = m_subscribes.find(sid.event_id);
		if (m_subscribes.end() != it)
		{
			SubcribeDataBase *sdb = it->second;
			sdb->Remove(sid.f_key);
		}
	}

private:
	std::unordered_map<int, SubcribeDataBase *> m_subscribes;
};
