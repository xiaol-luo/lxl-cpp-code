KLog = {}

--define const
KLog.LOG_LEVEL_DEBUG = 1
KLog.LOG_LEVEL_INFO = 2
KLog.LOG_LEVEL_WARNING = 3
KLog.LOG_LEVEL_ERROR = 4

-- define param and funciton
KLog.nLogLevel = 0

function KLog.SetLogLevel(logLevel)
	KLog.nLogLevel = logLevel
end

function KLog.Debug(msg)
	if KLog.nLogLevel > KLog.LOG_LEVEL_DEBUG then
		return
	end

	print(msg)
end